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

#include <android/app/ApplicationProcess.h>
#include <android/os/Bundle.h>

namespace android {
namespace os {

class ServiceChannel;
template<typename T> class ServiceMessageClient;
template<typename T> class ServiceMessageHost;

class ServiceObject {
    template<typename T> friend class ServiceMessageClient;
    template<typename T> friend class ServiceMessageHost;
public:
    enum CreationTag { Create, Import };

    template<typename T> static std::shared_ptr<T> create(Parcel parcel = Parcel())
    {
        return std::static_pointer_cast<T>(add(factory<T>().create(parcel), Create));
    }
    template<typename T> static std::shared_ptr<T> import(Parcel& parcel)
    {
        return std::static_pointer_cast<T>(add(factory<T>().import(parcel), Import));
    }
    static std::shared_ptr<ServiceObject> get(int32_t);
    virtual ~ServiceObject();

    template<typename T> static void initService() { factory<T>(); }

    int32_t objectUid() const { return m_objectUid; }

    void ref();
    void deref();

    virtual void createFromParcel(Parcel&);
    virtual void updateFromBundle(Bundle&, int64_t = System::getProcessId());
    virtual void readFromParcel(Parcel&);
    virtual void writeToParcel(Parcel&, int32_t flags) const;
    virtual void importToProcess(int64_t);
    virtual void removedFromProcess(int64_t);

protected:
    ServiceObject(ServiceChannel&);

    bool update(Bundle&);
    void notify(int64_t);

private:
    template<typename T> static ServiceMessageClient<T>& factory()
    {
        static std::lazy_ptr<ServiceMessageClient<T>> staticFactory([] { return ApplicationProcess::current().isRoot() ? new ServiceMessageHost<T> : new ServiceMessageClient<T>; });
        return *staticFactory.get();
    }
    static std::shared_ptr<ServiceObject> add(std::shared_ptr<ServiceObject>, CreationTag);

    ServiceChannel& m_channel;
    int32_t m_objectUid { 0 };
};

template<typename T, typename std::enable_if<std::is_base_of<ServiceObject, T>::value, T>::type* = 0>
Parcel& operator<<(Parcel& parcel, std::passed_ptr<T> object)
{
    int32_t objectUid = object ? object->objectUid() : 0;
    // Balanced by ServiceMessageClient<T>::import or ServiceMessageHost<T>::import.
    parcel << objectUid;
    return parcel;
}

template<typename T, typename std::enable_if<std::is_base_of<ServiceObject, T>::value, T>::type* = 0>
Parcel& operator>>(Parcel& parcel, std::shared_ptr<T>& object)
{
    object = os::ServiceObject::import<T>(parcel);
    return parcel;
}

} // namespace os
} // namespace android
