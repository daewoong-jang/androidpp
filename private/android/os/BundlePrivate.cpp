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

#include "BundlePrivate.h"

#include <android/os/ParcelablePrivate.h>

namespace android {
namespace os {

BundlePrivate& BundlePrivate::getPrivate(Bundle& bundle)
{
    return *bundle.m_private;
}

void BundlePrivate::setPrivate(Bundle& bundle, std::unique_ptr<BundlePrivate>&& bundlePrivate)
{
    bundle.m_private = std::move(bundlePrivate);
}

int32_t BundlePrivate::count()
{
    return m_keys.size();
}

bool BundlePrivate::findKey(StringRef key)
{
    return m_keys.count(key);
}

void BundlePrivate::clear()
{
    m_keys.clear();
    m_values.clear();
    m_charSequences.clear();
    m_parcelables.clear();
}

void BundlePrivate::remove(StringRef key)
{
    if (!findKey(key))
        return;

    removeKey(key);
}

BundleValue BundlePrivate::getValue(StringRef key)
{
    if (!findKey(key))
        return BundleValue();

    return m_values[key];
}

void BundlePrivate::putValue(StringRef key, BundleValue value)
{
    if (findKey(key))
        removeKey(key);

    m_values[key] = value;
    m_keys.insert(key);
}

CharSequence BundlePrivate::getCharSequence(StringRef key)
{
    if (!findKey(key))
        return L"";

    return *m_charSequences[key];
}

void BundlePrivate::putCharSequence(StringRef key, const CharSequence& value)
{
    if (findKey(key))
        removeKey(key);

    m_charSequences[key] = std::make_unique<CharSequence>(value);
    m_keys.insert(key);
}

std::shared_ptr<Parcelable> BundlePrivate::getParcelable(StringRef key)
{
    if (!findKey(key))
        return nullptr;

    return m_parcelables[key];
}

void BundlePrivate::putParcelable(StringRef key, std::passed_ptr<Parcelable> value)
{
    if (findKey(key))
        removeKey(key);

    m_parcelables[key] = value;
    m_keys.insert(key);
}

void BundlePrivate::writeToParcel(Parcel& dest, int32_t flags)
{
    dest << m_values.size();
    for(auto& value : m_values)
        dest << value.first << value.second.l;

    dest << m_charSequences.size();
    for(auto& charSequence : m_charSequences)
        dest << charSequence.first << *charSequence.second;

    dest << m_parcelables.size();
    for(auto& parcelable : m_parcelables) {
        dest << parcelable.first;
        parcelable.second->writeToParcel(dest, flags);
    }
}

void BundlePrivate::readFromParcel(Parcel& source)
{
    size_t values;
    source >> values;
    while (values-- > 0) {
        String key;
        BundleValue value;
        source >> key;
        m_keys.insert(key);
        source >> value.l;
        m_values[key] = value;
    }

    size_t charSequences;
    source >> charSequences;
    while (charSequences-- > 0) {
        String key;
        CharSequence value;
        source >> key;
        m_keys.insert(key);
        source >> value;
        m_charSequences[key] = std::make_unique<CharSequence>(std::move(value));
    }

    size_t parcelables;
    source >> parcelables;
    while (parcelables-- > 0) {
        String key;
        source >> key;
        m_keys.insert(key);
        m_parcelables[key] = ParcelablePrivate::createFromParcel(source);
    }
}

std::passed_ptr<Parcelable> BundlePrivate::getMessageObj()
{
    return m_messageObjHolder;
}

std::passed_ptr<Parcelable> BundlePrivate::setMessageObj(std::passed_ptr<Parcelable> obj)
{
    m_messageObjHolder = obj;
    return m_messageObjHolder;
}

void BundlePrivate::removeKey(StringRef key)
{
    assert(m_keys.count(key));

    if (m_values.count(key))
        m_values.erase(key);
    else if (m_charSequences.count(key))
        m_charSequences.erase(key);
    else if (m_parcelables.count(key))
        m_parcelables.erase(key);
    m_keys.erase(key);
}

} // namespace os
} // namespace android
