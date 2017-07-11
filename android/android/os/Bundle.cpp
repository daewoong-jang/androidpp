/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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

#include "Bundle.h"

#include <android/os/BundlePrivate.h>
#include <android/os/ParcelablePrivate.h>

namespace android {
namespace os {

Bundle::Bundle()
    : m_private(std::make_shared<BundlePrivate>())
{
}

Bundle::Bundle(const Bundle& o)
    : m_private(o.m_private)
{
}

Bundle::Bundle(Bundle&& o)
    : m_private(std::move(o.m_private))
{
}

Bundle& Bundle::operator=(const Bundle& other)
{
    m_private = other.m_private;
    return *this;
}

Bundle& Bundle::operator=(Bundle&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

int8_t Bundle::getByte(StringRef key)
{
    return m_private->getValue(key).b;
}

int8_t Bundle::getByte(StringRef key, int8_t defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getByte(key);
}

wchar_t Bundle::getChar(StringRef key)
{
    return m_private->getValue(key).c;
}

wchar_t Bundle::getChar(StringRef key, wchar_t defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getChar(key);
}

CharSequence Bundle::getCharSequence(StringRef key)
{
    return m_private->getCharSequence(key);
}

CharSequence Bundle::getCharSequence(StringRef key, const CharSequence& defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getCharSequence(key);
}

float Bundle::getFloat(StringRef key)
{
    return m_private->getValue(key).f;
}

float Bundle::getFloat(StringRef key, float defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getFloat(key);
}

int32_t Bundle::getInt(StringRef key)
{
    return m_private->getValue(key).i;
}

int32_t Bundle::getInt(StringRef key, int32_t defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getInt(key);
}

int16_t Bundle::getShort(StringRef key)
{
    return m_private->getValue(key).s;
}

int16_t Bundle::getShort(StringRef key, short defaultValue)
{
    if (!m_private->findKey(key))
        return defaultValue;

    return getShort(key);
}

void Bundle::putByte(StringRef key, int8_t value)
{
    m_private->putValue(key, BundleValue(value));
}

void Bundle::putChar(StringRef key, wchar_t value)
{
    m_private->putValue(key, BundleValue(value));
}

void Bundle::putCharSequence(StringRef key, const CharSequence& value)
{
    m_private->putCharSequence(key, value);
}

void Bundle::putFloat(StringRef key, float value)
{
    m_private->putValue(key, BundleValue(value));
}

void Bundle::putInt(StringRef key, int32_t value)
{
    m_private->putValue(key, BundleValue(value));
}

void Bundle::putShort(StringRef key, int16_t value)
{
    m_private->putValue(key, BundleValue(value));
}

std::shared_ptr<Parcelable> Bundle::getParcelable(StringRef key)
{
    return m_private->getParcelable(key);
}

void Bundle::putParcelable(StringRef key, std::passed_ptr<Parcelable> value)
{
    m_private->putParcelable(key, value);
}

void Bundle::readFromParcel(Parcel& parcel)
{
    m_private->readFromParcel(parcel);
}

void Bundle::clear()
{
    m_private->clear();
}

void Bundle::remove(StringRef key)
{
    m_private->remove(key);
}

bool Bundle::containsKey(StringRef key)
{
    return m_private->findKey(key);
}

class BundleCreator final : public ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        auto result = std::make_shared<Bundle>();
        result->readFromParcel(source);
        return result;
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    BundleCreator()
        : ParcelableCreator(this, L"android.os", L"Bundle.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> Bundle::CREATOR([] { return new BundleCreator; });

int32_t Bundle::describeContents()
{
    return 0;
}

void Bundle::writeToParcel(Parcel& dest, int32_t flags) const
{
    dest << ParcelableCreator::creator<Bundle>().binaryName;
    m_private->writeToParcel(dest, flags);
}

} // namespace os
} // namespace android
