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

#include "MediaSurface.h"

namespace com {
namespace example {

MediaSurface::MediaSurface(GLSurfaceView& context, OnCreateSurfaceTextureListener&& listener)
    : mContext(context)
    , mOnCreateSurfaceTextureListener(std::move(listener))
{
}

MediaSurface::~MediaSurface()
{
}

void MediaSurface::updateTexture(float mtx[16])
{
    if (mTextureName == 0)
        createSurfaceTexture();

    mSurfaceTexture->updateTexImage();
    mSurfaceTexture->getTransformMatrix(mtx);
}

int32_t MediaSurface::getTextureName()
{
    return mTextureName;
}

void MediaSurface::onFrameAvailable(SurfaceTexture& surfaceTexture)
{
    mContext.requestRender();
}

void MediaSurface::createSurfaceTexture()
{
    GLuint textures[1];
    glGenTextures(1, textures);

    mTextureName = textures[0];
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mTextureName);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,
            GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_EXTERNAL_OES,
            GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    mSurfaceTexture = std::make_shared<SurfaceTexture>(mTextureName);
    std::shared_ptr<Surface> surface = std::make_shared<Surface>(mSurfaceTexture);
    mContext.post([=] {
            mOnCreateSurfaceTextureListener(surface);
        });
}

} // namespace example
} // namespace com
