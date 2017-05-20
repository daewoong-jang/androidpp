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

#pragma once

#include <android/os/Parcel.h>
#include <android/os/Parcelable.h>

namespace android {
namespace os {

class BundlePrivate;

class Bundle : public Parcelable {
    friend class BundlePrivate;
public:
    ANDROID_EXPORT Bundle();
    ANDROID_EXPORT Bundle(const Bundle&);
    ANDROID_EXPORT Bundle(Bundle&&);
    ANDROID_EXPORT Bundle& operator=(const Bundle&);
    ANDROID_EXPORT Bundle& operator=(Bundle&&);
    ANDROID_EXPORT virtual ~Bundle() = default;

    // Returns the value associated with the given key, or (int8_t) 0 if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual int8_t getByte(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual int8_t getByte(StringRef key, int8_t defaultValue);
    // Returns the value associated with the given key, or (char) 0 if no mapping of the desired type exists for the given key.
    ANDROID_EXPORT virtual wchar_t getChar(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual wchar_t getChar(StringRef key, wchar_t defaultValue);
    // Returns the value associated with the given key, or null if no mapping of the desired type exists for the given key or a null value is explicitly associated with the key. 
    ANDROID_EXPORT virtual CharSequence getCharSequence(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key or if a null value is explicitly associatd with the given key.
    ANDROID_EXPORT virtual CharSequence getCharSequence(StringRef key, const CharSequence& defaultValue);
    // Returns the value associated with the given key, or 0.0f if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual float getFloat(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key.
    ANDROID_EXPORT virtual float getFloat(StringRef key, float defaultValue);
    // Returns the value associated with the given key, or 0 if no mapping of the desired type exists for the given key.
    ANDROID_EXPORT virtual int32_t getInt(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual int32_t getInt(StringRef key, int32_t defaultValue);
    // Returns the value associated with the given key, or (short) 0 if no mapping of the desired type exists for the given key.
    ANDROID_EXPORT virtual int16_t getShort(StringRef key);
    // Returns the value associated with the given key, or defaultValue if no mapping of the desired type exists for the given key. 
    ANDROID_EXPORT virtual int16_t getShort(StringRef key, short defaultValue);

    // Inserts a byte value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putByte(StringRef key, int8_t value);
    // Inserts a char value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putChar(StringRef key, wchar_t value);
    // Inserts a CharSequence value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putCharSequence(StringRef key, const CharSequence& value);
    // Inserts a float value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putFloat(StringRef key, float value);
    // Inserts an int value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putInt(StringRef key, int32_t value);
    // Inserts a short value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putShort(StringRef key, int16_t value);

    // Returns the value associated with the given key, or null if no mapping of the desired type exists for the given key or a null value is explicitly associated with the key.
    ANDROID_EXPORT virtual std::shared_ptr<Parcelable> getParcelable(StringRef key);
    // Inserts a Parcelable value into the mapping of this Bundle, replacing any existing value for the given key. 
    ANDROID_EXPORT virtual void putParcelable(StringRef key, std::passed_ptr<Parcelable> value);

    // Reads the Parcel contents into this Bundle, typically in order for it to be passed through an IBinder connection. 
    ANDROID_EXPORT virtual void readFromParcel(Parcel& parcel);

    // Removes all elements from the mapping of this Bundle.
    ANDROID_EXPORT virtual void clear();
    // Removes any entry with the given key from the mapping of this Bundle. 
    ANDROID_EXPORT virtual void remove(StringRef key);

    // BaseBundle
    // Returns true if the given key is contained in the mapping of this Bundle. 
    ANDROID_EXPORT virtual bool containsKey(StringRef key);

    // Parcelable
    ANDROID_EXPORT static const std::lazy_ptr<Parcelable::Creator> CREATOR;

    ANDROID_EXPORT virtual int32_t describeContents() override;
    ANDROID_EXPORT virtual void writeToParcel(Parcel& dest, int32_t flags);

private:
    std::shared_ptr<BundlePrivate> m_private;
};

} // namespace os
} // namespace android

using Bundle = android::os::Bundle;
