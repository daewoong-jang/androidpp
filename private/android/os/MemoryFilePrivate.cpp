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

#include "MemoryFilePrivate.h"

#include "ServiceMessageClient.h"
#include "ServiceMessageHost.h"
#include <android++/LogHelper.h>
#include <android++/StringConversion.h>

namespace android {
namespace os {

MemoryFilePrivate::MemoryFilePrivate(ServiceMessageClient<MemoryFilePrivate>& client)
    : ServiceObject(client)
{
}

MemoryFilePrivate::~MemoryFilePrivate()
{
    close();
}

void MemoryFilePrivate::initService()
{
    ServiceObject::initService<MemoryFilePrivate>();
}

enum Update : int32_t { None, Map, Close };

bool MemoryFilePrivate::map(const std::string& name, int32_t length)
{
    m_data = nullptr;

    Bundle data;
    data.putInt(L"type", Update::Map);
    data.putCharSequence(L"name", std::s2ws(name));
    data.putInt(L"length", length);

    if (!update(data)) {
        LOGE("Failed to map memory to file: requested length=%d", length);
        return false;
    }
    return true;
}

void MemoryFilePrivate::close()
{
    if (!handle())
        return;

    Bundle data;
    data.putInt(L"type", Update::Close);
    if (!update(data))
        LOGE("Closing memory file on handle %d did fail", handle());
}

void* MemoryFilePrivate::data()
{
    return m_data;
}

void MemoryFilePrivate::createFromParcel(Parcel& data)
{
    ServiceObject::createFromParcel(data);
}

void MemoryFilePrivate::updateFromBundle(Bundle& data, int64_t senderPid)
{
    ServiceObject::updateFromBundle(data, senderPid);

    int32_t type = data.getInt(L"type");
    switch (type) {
    case Update::Map: {
        std::string name = std::ws2s(data.getCharSequence(L"name"));
        int32_t length = data.getInt(L"length");

        assert_wtf(!handle());

        setHandle(platformMap(name.length() > 0 ? name.c_str() : 0, length));
        if (!handle())
            break;

        m_name = name;
        m_length = length;
        m_data = platformData();
        return;
    }
    case Update::Close:
        assert_wtf(!handle());
        m_name.clear();
        m_length = 0;
        PlatformHandle::close();
        m_data = nullptr;
        return;
    default:
        break;
    }

    LOGE("Error on memory file update");
}

void MemoryFilePrivate::readFromParcel(Parcel& source)
{
    ServiceObject::readFromParcel(source);
    PlatformHandle::readFromParcel(source);

    source >> m_name;
    source >> m_length;

    m_data = platformData();
}

void MemoryFilePrivate::writeToParcel(Parcel& dest, int32_t flags) const
{
    ServiceObject::writeToParcel(dest, flags);
    PlatformHandle::writeToParcel(dest, flags);

    dest << m_name;
    dest << m_length;
}

} // namespace os
} // namespace android
