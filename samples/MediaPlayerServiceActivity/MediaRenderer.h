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
#include <android/opengl/GLSurfaceView.h>

namespace com {
namespace example {

class MediaSurface;

class MediaRenderer : public Object, public GLSurfaceView::Renderer {
public:
    MediaRenderer(Context& context);
    ~MediaRenderer();

    void setSurface(std::passed_ptr<MediaSurface> surface);

    void onSurfaceCreated(GL10 gl) override;
    void onSurfaceChanged(GL10 gl, int32_t width, int32_t height) override;
    void onDrawFrame(GL10 gl) override;

    int32_t loadShader(int32_t shaderType, std::string source);
    int32_t createProgram(std::string vertexSource, std::string fragmentSource);
    void checkGlError(std::string op);

private:
    std::vector<float> mTriangleVertices;

    float mMVPMatrix[16];
    float mSTMatrix[16];

    int32_t mProgram;
    int32_t muMVPMatrixHandle;
    int32_t muSTMatrixHandle;
    int32_t maPositionHandle;
    int32_t maTextureHandle;

    std::shared_ptr<MediaSurface> mSurface;
};

} // namespace example
} // namespace com
