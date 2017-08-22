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

#include "SurfaceTexture.h"

#include <android/graphics/SurfaceTexturePrivate.h>
#include <android/os/ParcelablePrivate.h>

namespace android {
namespace graphics {

SurfaceTexture::SurfaceTexture(int32_t texName)
    : m_private(std::make_shared<SurfaceTexturePrivate>(*this, texName))
{
}

SurfaceTexture::SurfaceTexture(const SurfaceTexture& other)
    : m_private(other.m_private)
{
}

SurfaceTexture::SurfaceTexture(SurfaceTexture&& other)
    : m_private(std::move(other.m_private))
{
}

SurfaceTexture& SurfaceTexture::operator=(const SurfaceTexture& other)
{
    m_private = other.m_private;
    return *this;
}

SurfaceTexture& SurfaceTexture::operator=(SurfaceTexture&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

SurfaceTexture::~SurfaceTexture()
{
}

void SurfaceTexture::getTransformMatrix(float mtx[16])
{
    m_private->getTransformMatrix(mtx);
}

void SurfaceTexture::release()
{
    m_private->release();
}

void SurfaceTexture::setOnFrameAvailableListener(OnFrameAvailableListener* listener)
{
    m_private->setOnFrameAvailableListener(listener, nullptr);
}

void SurfaceTexture::setOnFrameAvailableListener(OnFrameAvailableListener* listener, Handler* handler)
{
    m_private->setOnFrameAvailableListener(listener, handler);
}

void SurfaceTexture::updateTexImage()
{
    m_private->updateTexImage();
}

} // namespace graphics
} // namespace android
