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

#include <android/os/Parcelable.h>

namespace android {
namespace os {

class ParcelablePrivate final {
public:
    static std::shared_ptr<Parcelable> createFromParcel(Parcel& source);
    static std::shared_ptr<Parcelable> createFromParcel(Parcel& source, StringRef binaryName);
};

class ParcelableCreator : public Parcelable::Creator {
protected:
    template<typename T>
    ParcelableCreator(T*, String&& packageName, String&& name)
        : binaryName(packageName + L'.' + name)
    {
        ClassLoader::getSystemClassLoader().definePackage(String(packageName), String(), String(), String(), String(), String(), String(), URL());
        ClassLoader::getSystemClassLoader().resolveClass(std::make_shared<java::lang::ClassT<T>>(std::move(packageName), std::move(name)));
    }

public:
    template<typename T> static ParcelableCreator& creator()
    {
        return static_cast<ParcelableCreator&>(*T::CREATOR);
    }

    template<typename T> static void writeToParcel(T&, Parcel& dest)
    {
        dest << ParcelableCreator::creator<T>().binaryName;
    }

    const String binaryName;
};

} // namespace os
} // namespace android

using ParcelableCreator = android::os::ParcelableCreator;
using ParcelablePrivate = android::os::ParcelablePrivate;
