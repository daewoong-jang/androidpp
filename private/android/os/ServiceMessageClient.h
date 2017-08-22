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

#pragma once

#include <android/app/MessageClientBase.h>
#include <android/content/ApplicationLauncher.h>
#include <android/os/ServiceChannel.h>
#include <android/os/ServiceObject.h>
#include <android++/LogHelper.h>

namespace android {
namespace os {

template<typename T>
class ServiceMessageClient : public MessageClientBase<T>, public ServiceChannel {
public:
    static const int32_t CREATE_OBJECT = E(1);
    static const int32_t IMPORT_OBJECT = E(2);
    static const int32_t UPDATE_OBJECT = E(3);
    static const int32_t NOTIFY_OBJECT = E(4);
    static const int32_t REMOVE_OBJECT = E(5);

    ServiceMessageClient();
    virtual ~ServiceMessageClient();

    virtual std::shared_ptr<T> create(Parcel&);
    virtual std::shared_ptr<T> import(Parcel&);

private:
    // ServiceChannel
    virtual void import(int32_t) override;
    virtual bool update(int32_t, Bundle&) override;
    virtual bool notify(int32_t, int64_t, int64_t) override;
    virtual void remove(int32_t) override;

    // ApplicationProcess:MessageClient
    virtual void onTimer() override;
    virtual bool onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;
};

template<typename T>
ServiceMessageClient<T>::ServiceMessageClient()
{
    ApplicationProcess::current().appendMessageClient(this);
}

template<typename T>
ServiceMessageClient<T>::~ServiceMessageClient()
{
    ApplicationProcess::current().removeMessageClient(this);
}

template<typename T>
std::shared_ptr<T> ServiceMessageClient<T>::create(Parcel& data)
{
    Parcel reply;
    if (!ApplicationProcess::current().root()->transact(CREATE_OBJECT, data, &reply, 0))
        return nullptr;
    auto object = ServiceObject::import<T>(reply);
    return object;
}

template<typename T>
std::shared_ptr<T> ServiceMessageClient<T>::import(Parcel& data)
{
    int32_t objectUid;
    data >> objectUid;
    if (objectUid == 0)
        return nullptr;

    auto object = ServiceObject::get(objectUid);
    if (!object) {
        object = std::make_shared<T>(*this);
        object->m_objectUid = objectUid;
    }
    return std::static_pointer_cast<T>(object);
}

template<typename T>
void ServiceMessageClient<T>::import(int32_t objectUid)
{
    Parcel parcel;
    parcel << objectUid;
    parcel << System::getProcessId();

    Parcel reply;
    if (!ApplicationProcess::current().root()->transact(IMPORT_OBJECT, parcel, &reply, 0))
        LOGA("Couldn't transact with service host");

    auto object = ServiceObject::get(objectUid);
    object->readFromParcel(reply);
}

template<typename T>
bool ServiceMessageClient<T>::update(int32_t objectUid, Bundle& data)
{
    Parcel parcel;
    parcel << objectUid;
    parcel << System::getProcessId();
    parcel << data;

    Parcel reply;
    if (!ApplicationProcess::current().root()->transact(UPDATE_OBJECT, parcel, &reply, 0)) {
        LOGA("Couldn't transact with service host");
        return false;
    }
    auto object = ServiceObject::get(objectUid);
    object->readFromParcel(reply);
    return true;
}

template<typename T>
bool ServiceMessageClient<T>::notify(int32_t objectUid, int64_t senderPid, int64_t destinationPid)
{
    assert(false);
    return false;
}

template<typename T>
void ServiceMessageClient<T>::remove(int32_t objectUid)
{
    Parcel parcel;
    parcel << objectUid;
    parcel << System::getProcessId();

    if (!ApplicationProcess::current().root()->transact(REMOVE_OBJECT, parcel, &parcel, 0))
        LOGA("Couldn't transact with service host");
}

template<typename T>
void ServiceMessageClient<T>::onTimer()
{
}

template<typename T>
bool ServiceMessageClient<T>::onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    switch (code) {
    case NOTIFY_OBJECT: {
        int32_t objectUid;
        data >> objectUid;

        int64_t senderPid;
        data >> senderPid;
        release_assert(senderPid != System::getProcessId());

        auto object = ServiceObject::get(objectUid);
        if (object)
            object->readFromParcel(data);
        return true;
    }
    default:
        break;
    }
    return false;
}

} // namespace os
} // namespace android
