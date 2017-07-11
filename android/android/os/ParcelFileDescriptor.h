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

class ParcelFileDescriptor : public Parcelable {
    NONCOPYABLE(ParcelFileDescriptor);
    friend class ParcelFileDescriptorCreator;
public:
    ANDROID_EXPORT static const std::lazy_ptr<Parcelable::Creator> CREATOR;

    ANDROID_EXPORT ParcelFileDescriptor();
    ANDROID_EXPORT virtual ~ParcelFileDescriptor();

    // Take ownership of a raw native fd in to a new ParcelFileDescriptor. 
    ANDROID_EXPORT static std::shared_ptr<ParcelFileDescriptor> adoptFd(int32_t fd);
    // Return the native fd int for this ParcelFileDescriptor and detach it from the object here.
    ANDROID_EXPORT virtual int32_t detachFd();

    // Describe the kinds of special objects contained in this Parcelable instance's marshaled representation.
    ANDROID_EXPORT virtual int32_t describeContents() override;
    // Flatten this object in to a Parcel. If PARCELABLE_WRITE_RETURN_VALUE is set in flags, the file descriptor will be closed after a copy is written to the Parcel. 
    ANDROID_EXPORT virtual void writeToParcel(Parcel& dest, int32_t flags) const override;

private:
    int32_t m_fd { 0 };
    intptr_t m_handle { 0 };
    intptr_t m_sourcePid { 0 };
    bool m_close { false };
};

} // namespace os
} // namespace android

using ParcelFileDescriptor = android::os::ParcelFileDescriptor;
