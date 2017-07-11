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

#include <android/os/Parcel.h>

namespace android {
namespace os {

class Parcelable {
public:
    // Descriptor bit used with describeContents(): indicates that the Parcelable object's flattened representation includes a file descriptor.
    ANDROID_EXPORT static const int32_t CONTENTS_FILE_DESCRIPTOR = 1;
    // Flag for use with writeToParcel(Parcel, int): the object being written is a return value, that is the result of a function such as "Parcelable someFunction()", "void someFunction(out Parcelable)", or "void someFunction(inout Parcelable)". 
    ANDROID_EXPORT static const int32_t PARCELABLE_WRITE_RETURN_VALUE = 1;

    // Interface that must be implemented and provided as a public CREATOR field that generates instances of your Parcelable class from a Parcel.
    class Creator {
    public:
        virtual ~Creator() = default;
        // Create a new instance of the Parcelable class, instantiating it from the given Parcel whose data had previously been written by Parcelable.writeToParcel().
        virtual std::shared_ptr<Parcelable> createFromParcel(Parcel& source) = 0;
        // Create a new array of the Parcelable class. 
        virtual std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) = 0;
    };

    ANDROID_EXPORT virtual ~Parcelable() = default;

    // Describe the kinds of special objects contained in this Parcelable instance's marshaled representation.
    virtual int32_t describeContents() = 0;
    // Flatten this object in to a Parcel.
    virtual void writeToParcel(Parcel& dest, int32_t flags) const = 0;

protected:
    ANDROID_EXPORT Parcelable() = default;
};

} // namespace os
} // namespace android

using Parcelable = android::os::Parcelable;
