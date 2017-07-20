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
#include <android/os/Handler.h>
#include <android/os/Looper.h>

namespace android {
namespace graphics {

class SurfaceTexture;

class SurfaceTexturePrivate {
    friend class SurfaceTexture;
public:
    SurfaceTexturePrivate(SurfaceTexture&, int32_t texName);
    ~SurfaceTexturePrivate();

    static SurfaceTexturePrivate& getPrivate(SurfaceTexture&);
    static void setPrivate(SurfaceTexture&, std::unique_ptr<SurfaceTexturePrivate>&&);

    SurfaceTexture& getThis() { return m_thisTexture; }
    int32_t getTexName() const { return m_texName; }

    void getTransformMatrix(float mtx[16]);
    void release();
    void setOnFrameAvailableListener(SurfaceTexture::OnFrameAvailableListener* listener, Handler* handler);
    void updateTexImage();

private:
    SurfaceTexture& m_thisTexture;
    Looper* m_creatorLooper;
    std::unique_ptr<Handler> m_onFrameAvailableHandler;
    int32_t m_texName;
};

} // namespace graphics
} // namespace android
