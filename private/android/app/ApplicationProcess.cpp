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

#include "ApplicationProcess.h"

#include <android/view/SurfacePrivate.h>

namespace android {
namespace app {

ApplicationProcess& ApplicationProcess::current()
{
    static ApplicationProcess* currentProcess = new ApplicationProcess();
    return *currentProcess;
}

ApplicationProcess::ApplicationProcess()
    : m_self(Binder::create(*this))
    , m_mainThreadHandler(std::make_shared<Handler>())
{
    platformCreate();
}

ApplicationProcess::~ApplicationProcess()
{
    platformDestroy();
}

bool ApplicationProcess::isRoot()
{
    return m_root == m_self;
}

std::passed_ptr<Binder> ApplicationProcess::self()
{
    return m_self;
}

std::passed_ptr<Binder> ApplicationProcess::root()
{
    return m_root;
}

bool ApplicationProcess::initialize(intptr_t root, std::unordered_map<String, String>& parameters)
{
    if (m_root)
        return true;

    m_root = root ? Binder::adopt(root) : m_self;

    view::SurfacePrivate::initService();

    return platformInitialize(parameters);
}

intptr_t ApplicationProcess::handle()
{
    return m_self->handle();
}

bool ApplicationProcess::setTimeout()
{
    return m_self->start();
}

bool ApplicationProcess::setTimeout(std::chrono::milliseconds time)
{
    return m_self->startAtTime(time);
}

bool ApplicationProcess::post(std::function<void ()>&& r)
{
    return m_mainThreadHandler->post(std::move(r));
}

bool ApplicationProcess::postDelayed(std::function<void ()>&& r, std::chrono::milliseconds delayMillis)
{
    return m_mainThreadHandler->postDelayed(std::move(r), delayMillis);
}

void ApplicationProcess::appendMessageClient(MessageClient* client)
{
    m_messageClients.push_back(client);
}

void ApplicationProcess::removeMessageClient(MessageClient* client)
{
    m_messageClients.erase(std::find(m_messageClients.begin(), m_messageClients.end(), client));
}

void ApplicationProcess::onCreate()
{
}

void ApplicationProcess::onDestroy()
{
}

void ApplicationProcess::onTimer()
{
    for (auto messageClient : m_messageClients)
        messageClient->onTimer();
}

void ApplicationProcess::onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    for (auto messageClient : m_messageClients) {
        if (messageClient->onTransaction(code, data, reply, flags))
            return;
    }
}

} // namespace app
} // namespace android
