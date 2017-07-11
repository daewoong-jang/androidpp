/*
 * Copyright (C) 2017 Daewoong Jang.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Binder.h"

#include "BinderProvider.h"
#include <android/app/ApplicationProcess.h>
#include <android/os/ParcelPrivate.h>
#include <android++/LogHelper.h>
#include <android++/TemporaryChange.h>

namespace android {
namespace os {

class BinderProxy : public Binder {
public:
    BinderProxy(intptr_t, Client*);
    virtual ~BinderProxy();

    intptr_t handle() const override { return m_handle; }
    app::HostWindow* window() override { return nullptr; }

    virtual bool transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;

protected:
    intptr_t m_handle;
};

BinderProxy::BinderProxy(intptr_t handle, Client* client)
    : Binder(client)
    , m_handle(handle)
{
}

BinderProxy::~BinderProxy()
{
}

bool BinderProxy::transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    return ApplicationProcess::current().self()->transact(this, code, data, reply, flags);
}

class LocalBinder final : public BinderProxy, public BinderProvider::Client {
public:
    LocalBinder(Binder::Client*);
    ~LocalBinder();

    bool isLocal() const override;

    bool start() override;
    bool startAtTime(std::chrono::milliseconds) override;
    void stop() override;

    void close() override;

    bool transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;

private:
    // Binder
    bool transact(Binder* destination, int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;

    // BinderProvider::Client
    void onCreate() override;
    void onDestroy() override;
    void onTimer() override;
    void onTransaction(int32_t code, Parcel& data, intptr_t replyTo, int32_t flags) override;

    std::unique_ptr<BinderProvider> m_provider;
    Parcel* m_reply { nullptr };
};

LocalBinder::LocalBinder(Binder::Client* client)
    : BinderProxy(0, client)
    , m_provider(BinderProvider::create(*this))
{
    m_handle = m_provider->handle();
}

LocalBinder::~LocalBinder()
{
}

bool LocalBinder::isLocal() const
{
    return true;
}

bool LocalBinder::start()
{
    return m_provider->start();
}

bool LocalBinder::startAtTime(std::chrono::milliseconds uptimeMillis)
{
    return m_provider->startAtTime(uptimeMillis);
}

void LocalBinder::stop()
{
    m_provider->stop();
}

void LocalBinder::close()
{
    m_provider->close();
}

bool LocalBinder::transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    LOGV("Transaction attempt on local binder %x", this);
    m_client->onTransaction(code, data, reply, flags);
    return true;
}

bool LocalBinder::transact(Binder* destination, int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    TemporaryChange<Parcel*> replyChange(m_reply, reply);
    return m_provider->transact(destination, code, data, flags);
}

void LocalBinder::onCreate()
{
    m_client->onCreate();
}

void LocalBinder::onDestroy()
{
    m_client->onDestroy();
}

void LocalBinder::onTimer()
{
    m_client->onTimer();
}

void LocalBinder::onTransaction(int32_t code, Parcel& data, intptr_t replyTo, int32_t flags)
{
    if (replyTo) {
        Parcel reply;
        m_client->onTransaction(code, data, &reply, 0);
        auto sender = Binder::adopt(replyTo);
        if (!sender->transact(Binder::REPLY_TRANSACTION, reply, nullptr, IBinder::FLAG_ONEWAY)) {
            LOGE("Couldn't send reply");
            return;
        }
    } else {
        if (code == Binder::REPLY_TRANSACTION) {
            assert(m_reply);
            *m_reply = data;
        } else {
            m_client->onTransaction(code, data, nullptr, IBinder::FLAG_ONEWAY);
        }
    }
}

class EmptyBinderClient final : public Binder::Client {
public:
    void onCreate() override { }
    void onDestroy() override { }
    void onTimer() override { }
    void onTransaction(int32_t, Parcel&, Parcel*, int32_t) override { }
};

std::shared_ptr<Binder> Binder::create(Client& client)
{
    return std::make_shared<LocalBinder>(&client);
}

std::shared_ptr<Binder> Binder::adopt(intptr_t handle)
{
    return std::make_shared<BinderProxy>(handle, nullptr);
}

Binder::Binder(Client* client)
{
    static EmptyBinderClient emptyClient;
    m_client = client ? client : &emptyClient;
}

bool Binder::transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    assert(false);
    return false;
}

bool Binder::transact(Binder* destination, int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    assert(false);
    return false;
}

} // namespace os
} // namespace android
