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

#include "Parcel.h"

#include <android/os/ParcelPrivate.h>
#include <android/os/ParcelablePrivate.h>

namespace android {
namespace os {

Parcel::Parcel()
    : m_private(std::make_shared<ParcelPrivate>(*this))
{
    m_private->reset();
}

Parcel::Parcel(const Parcel& other)
    : m_private(other.m_private)
{
}

Parcel::Parcel(Parcel&& other)
    : m_private(std::move(other.m_private))
{
}

Parcel& Parcel::operator=(const Parcel& other)
{
    m_private = other.m_private;
    return *this;
}

Parcel& Parcel::operator=(Parcel&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

Parcel::~Parcel()
{
}

int32_t Parcel::dataSize()
{
    return m_private->size();
}

int8_t* Parcel::data()
{
    return m_private->data();
}

Parcel& Parcel::operator<<(bool value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(wchar_t value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(int8_t value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(int32_t value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(int64_t value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(size_t value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(float value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(double value)
{
    m_private->write(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator<<(const std::string& value)
{
    m_private->writeArray(value.data(), value.length(), sizeof(std::string::value_type));
    return *this;
}

Parcel& Parcel::operator<<(const CharSequence& value)
{
    m_private->writeArray(value.data(), value.length(), sizeof(CharSequence::value_type));
    return *this;
}

Parcel& Parcel::operator<<(const Parcelable& value)
{
    value.writeToParcel(*this, 0);
    return *this;
}

Parcel& Parcel::operator>>(bool& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(wchar_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(int8_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(uint16_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(int32_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(int64_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(size_t& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(float& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(double& value)
{
    m_private->read(&value, sizeof(value), sizeof(value));
    return *this;
}

Parcel& Parcel::operator>>(std::string& value)
{
    size_t length = 0;
    int8_t* data = m_private->readArray(length, sizeof(std::string::value_type));
    if (!data)
        return *this;

    value.assign(std::string(reinterpret_cast<std::string::value_type*>(data), length));
    return *this;
}

Parcel& Parcel::operator>>(CharSequence& value)
{
    size_t length = 0;
    int8_t* data = m_private->readArray(length, sizeof(CharSequence::value_type));
    if (!data)
        return *this;

    value.assign(CharSequence(reinterpret_cast<CharSequence::value_type*>(data), length));
    return *this;
}

Parcel& Parcel::operator>>(std::shared_ptr<Parcelable>& value)
{
    value = ParcelablePrivate::createFromParcel(*this);
    return *this;
}

} // namespace os
} // namespace android
