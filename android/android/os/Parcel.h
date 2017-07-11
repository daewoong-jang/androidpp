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

#include <java/lang.h>

namespace android {
namespace os {

class ParcelPrivate;
class Parcelable;

class Parcel final {
    friend class ParcelPrivate;
public:
    ANDROID_EXPORT Parcel();
    ANDROID_EXPORT Parcel(const Parcel&);
    ANDROID_EXPORT Parcel(Parcel&&);
    ANDROID_EXPORT Parcel& operator=(const Parcel&);
    ANDROID_EXPORT Parcel& operator=(Parcel&&);
    ANDROID_EXPORT ~Parcel();

    // Returns the total amount of data contained in the parcel. 
    ANDROID_EXPORT int32_t dataSize();

    ANDROID_EXPORT int8_t* data();

    ANDROID_EXPORT Parcel& operator<<(bool);
    ANDROID_EXPORT Parcel& operator<<(wchar_t);
    ANDROID_EXPORT Parcel& operator<<(int8_t);
    ANDROID_EXPORT Parcel& operator<<(int32_t);
    ANDROID_EXPORT Parcel& operator<<(int64_t);
    ANDROID_EXPORT Parcel& operator<<(size_t);
    ANDROID_EXPORT Parcel& operator<<(float);
    ANDROID_EXPORT Parcel& operator<<(double);
    ANDROID_EXPORT Parcel& operator<<(const std::string&);
    ANDROID_EXPORT Parcel& operator<<(const CharSequence&);
    ANDROID_EXPORT Parcel& operator<<(const Parcelable&);

    ANDROID_EXPORT Parcel& operator>>(bool&);
    ANDROID_EXPORT Parcel& operator>>(wchar_t&);
    ANDROID_EXPORT Parcel& operator>>(int8_t&);
    ANDROID_EXPORT Parcel& operator>>(uint16_t&);
    ANDROID_EXPORT Parcel& operator>>(int32_t&);
    ANDROID_EXPORT Parcel& operator>>(int64_t&);
    ANDROID_EXPORT Parcel& operator>>(size_t&);
    ANDROID_EXPORT Parcel& operator>>(float&);
    ANDROID_EXPORT Parcel& operator>>(double&);
    ANDROID_EXPORT Parcel& operator>>(std::string&);
    ANDROID_EXPORT Parcel& operator>>(CharSequence&);
    ANDROID_EXPORT Parcel& operator>>(std::shared_ptr<Parcelable>&);

private:
    std::shared_ptr<ParcelPrivate> m_private;
};

} // namespace os
} // namespace android

using Parcel = android::os::Parcel;
