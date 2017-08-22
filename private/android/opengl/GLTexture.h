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

#include "GLTextureHandle.h"
#include <android/graphics/Point.h>
#include <android/graphics/Rect.h>

namespace android {
namespace opengl {

class GLTexture : public GLTextureHandle {
public:
    enum class Format {
        RGBA = GL_RGBA,
        BGRA = 0x80E1,
    };

    GLTexture();
    GLTexture(GLuint);
    virtual ~GLTexture();

    void image2D(int32_t width, int32_t height, Format format, const int32_t* sourceData);
    void subImage2D(int32_t x, int32_t y, int32_t width, int32_t height, Format format, const int32_t* sourceData);

    int32_t width() { return m_width; }
    int32_t height() { return m_height; }

    GLTexture* texture() override { return this; }

protected:
    void upload(const int32_t* sourceData, Rect& targetRect, Point& sourceOffset, int32_t bytesPerLine, Format format);
    void upload(const int32_t* sourceData, Rect& targetRect, Point& sourceOffset, int32_t bytesPerLine, Format format, bool readOnly);

    int32_t m_width;
    int32_t m_height;
};

} // namespace opengl
} // namespace android
