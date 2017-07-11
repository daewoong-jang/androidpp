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

#include <android/graphics/SurfaceTexture.h>
#include <android/os/Parcelable.h>

namespace android {
namespace view {

class SurfacePrivate;

class Surface : public Parcelable {
    friend class SurfaceCreator;
    friend class SurfacePrivate;
public:
    // Rotation constant: 0 degree rotation (natural orientation)
    static const int32_t ROTATION_0 = 0;
    // Rotation constant: 90 degree rotation. 
    static const int32_t ROTATION_90 = 1;
    // Rotation constant: 180 degree rotation.
    static const int32_t ROTATION_180 = 2;
    // Rotation constant: 270 degree rotation.
    static const int32_t ROTATION_270 = 3;

    ANDROID_EXPORT Surface();
    // Create Surface from a SurfaceTexture. 
    ANDROID_EXPORT Surface(std::passed_ptr<SurfaceTexture>);
    ANDROID_EXPORT Surface(const Surface&);
    ANDROID_EXPORT Surface(Surface&&);
    ANDROID_EXPORT Surface& operator=(const Surface&);
    ANDROID_EXPORT Surface& operator=(Surface&&);
    ANDROID_EXPORT virtual ~Surface();

    // Returns true if this object holds a valid surface. 
    ANDROID_EXPORT virtual bool isValid();
    // Release the local reference to the server-side surface. 
    ANDROID_EXPORT virtual void release();

    ANDROID_EXPORT virtual void readFromParcel(Parcel& source);

    // Parcelable
    ANDROID_EXPORT static const std::lazy_ptr<Parcelable::Creator> CREATOR;

    ANDROID_EXPORT int32_t describeContents() override;
    ANDROID_EXPORT void writeToParcel(Parcel& dest, int32_t flags) const override;

private:
    enum UninitializedTag { Uninitialized };
    ANDROID_EXPORT Surface(UninitializedTag);

    std::shared_ptr<SurfacePrivate> m_private;
};

} // namespace view
} // namespace android

using Surface = android::view::Surface;
