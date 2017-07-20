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

#include <android/os/Handler.h>

namespace android {
namespace graphics {

class SurfaceTexturePrivate;

class SurfaceTexture {
    friend class SurfaceTexturePrivate;
public:
    class OnFrameAvailableListener {
    public:
        virtual void onFrameAvailable(SurfaceTexture& surfaceTexture) = 0;
    };

    // Construct a new SurfaceTexture to stream images to a given OpenGL texture. 
    ANDROID_EXPORT SurfaceTexture(int32_t texName);
    ANDROID_EXPORT SurfaceTexture(const SurfaceTexture&);
    ANDROID_EXPORT SurfaceTexture(SurfaceTexture&&);
    ANDROID_EXPORT SurfaceTexture& operator=(const SurfaceTexture&);
    ANDROID_EXPORT SurfaceTexture& operator=(SurfaceTexture&&);
    ANDROID_EXPORT virtual ~SurfaceTexture();

    // Retrieve the 4x4 texture coordinate transform matrix associated with the texture image set by the most recent call to updateTexImage.
    ANDROID_EXPORT virtual void getTransformMatrix(float mtx[16]);
    // release() frees all the buffers and puts the SurfaceTexture into the 'abandoned' state.
    ANDROID_EXPORT virtual void release();
    // Register a callback to be invoked when a new image frame becomes available to the SurfaceTexture.
    ANDROID_EXPORT virtual void setOnFrameAvailableListener(OnFrameAvailableListener* listener);
    ANDROID_EXPORT virtual void setOnFrameAvailableListener(OnFrameAvailableListener* listener, Handler* handler);
    // Update the texture image to the most recent frame from the image stream. 
    ANDROID_EXPORT virtual void updateTexImage();

private:
    std::shared_ptr<SurfaceTexturePrivate> m_private;
};

} // namespace graphics
} // namespace android

using SurfaceTexture = android::graphics::SurfaceTexture;
