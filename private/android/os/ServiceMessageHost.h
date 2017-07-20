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

#include <android/os/ServiceMessageClient.h>
#include <android/os/ServiceObjectRef.h>
#include <unordered_map>

namespace android {
namespace os {

template<typename T>
class ServiceMessageHost final : public ServiceMessageClient<T> {
public:
    ServiceMessageHost();
    ~ServiceMessageHost();

    std::shared_ptr<T> create(Parcel&) override;
    std::shared_ptr<T> import(Parcel&) override;

private:
    // ServiceChannel
    void import(int32_t) override;
    void ref(int32_t) override;
    void deref(int32_t) override;
    bool update(int32_t, Bundle&) override;
    bool notify(int32_t, int64_t, int64_t) override;

    // ApplicationProcess:MessageClient
    void onTimer() override;
    bool onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;
};

template<typename T>
ServiceMessageHost<T>::ServiceMessageHost()
{
}

template<typename T>
ServiceMessageHost<T>::~ServiceMessageHost()
{
}

template<typename T>
std::shared_ptr<T> ServiceMessageHost<T>::create(Parcel& data)
{
    auto object = std::make_shared<T>(*this);
    object->createFromParcel(data);
    return object;
}

template<typename T>
std::shared_ptr<T> ServiceMessageHost<T>::import(Parcel& data)
{
    assert(false);
    return nullptr;
}

template<typename T>
void ServiceMessageHost<T>::import(int32_t)
{
}

template<typename T>
void ServiceMessageHost<T>::ref(int32_t objectUid)
{
    ServiceObject::get(objectUid)->ref();
}

template<typename T>
void ServiceMessageHost<T>::deref(int32_t objectUid)
{
    ServiceObject::get(objectUid)->deref();
}

template<typename T>
bool ServiceMessageHost<T>::update(int32_t objectUid, Bundle& data)
{
    auto object = ServiceObject::get(objectUid);
    assert(object);
    object->updateFromBundle(data);
    object->notify(System::getProcessId());
    return true;
}

template<typename T>
bool ServiceMessageHost<T>::notify(int32_t objectUid, int64_t senderPid, int64_t destinationPid)
{
    auto object = ServiceObject::get(objectUid);
    auto target = ApplicationLauncher::get().remote(destinationPid);
    if (!target)
        return false;
    Parcel parcel;
    parcel << objectUid;
    parcel << senderPid;
    object->writeToParcel(parcel, 0);
    if (!target->transact(NOTIFY_OBJECT, parcel, nullptr, 0)) {
        LOGA("Couldn't notify process %lld for service object %d", destinationPid, objectUid);
        return false;
    }
    return true;
}

template<typename T>
void ServiceMessageHost<T>::onTimer()
{
}

template<typename T>
bool ServiceMessageHost<T>::onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    switch (code) {
    case CREATE_OBJECT: {
        std::shared_ptr<T> object = ServiceObject::create<T>(data);
        object->writeToParcel(*reply, 0);
        return true;
    }
    case IMPORT_OBJECT:
    case SECURE_OBJECT:
    case UPDATE_OBJECT:
    case REMOVE_OBJECT: {
        int32_t objectUid;
        data >> objectUid;

        auto object = ServiceObject::get(objectUid);
        assert(object);

        switch (code) {
        case IMPORT_OBJECT: {
            int64_t remotePid;
            data >> remotePid;
            object->importToProcess(remotePid);
            return true;
        }
        case SECURE_OBJECT: {
            ref(objectUid);
            return true;
        }
        case UPDATE_OBJECT: {
            int64_t senderPid;
            data >> senderPid;

            std::shared_ptr<Parcelable> parcelable;
            data >> parcelable;

            object->updateFromBundle(*std::static_pointer_cast<Bundle>(parcelable), senderPid);
            object->notify(senderPid);
            object->writeToParcel(*reply, 0);
            return true;
        }
        case REMOVE_OBJECT: {
            deref(objectUid);
            return true;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
    return false;
}

} // namespace os
} // namespace android
