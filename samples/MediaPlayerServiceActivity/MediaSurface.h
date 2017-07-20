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

#include <android.h>
#include <android/graphics/SurfaceTexture.h>
#include <android/opengl/GLSurfaceView.h>
#include <android/view/Surface.h>

namespace com {
namespace example {

class MediaSurface : public SurfaceTexture::OnFrameAvailableListener {
public:
    using OnCreateSurfaceTextureListener = std::function<void (std::shared_ptr<Surface>)>;

    MediaSurface(GLSurfaceView& context, OnCreateSurfaceTextureListener&&);
    ~MediaSurface();

    void updateTexture(float mtx[16]);
    int32_t getTextureName();

    void onFrameAvailable(SurfaceTexture& surfaceTexture) override;

private:
    void createSurfaceTexture();

    GLSurfaceView& mContext;
    OnCreateSurfaceTextureListener mOnCreateSurfaceTextureListener;
    int32_t mTextureName = 0;
    std::shared_ptr<SurfaceTexture> mSurfaceTexture;
};

} // namespace example
} // namespace com
