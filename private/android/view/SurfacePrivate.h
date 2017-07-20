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

#include <android/os/Binder.h>
#include <android/os/MemoryFilePrivate.h>
#include <android/os/ServiceMessageClient.h>
#include <android/view/Surface.h>

namespace android {
namespace view {

class Surface;

class SurfacePrivate final : public os::ServiceObject {
    friend class Surface;
public:
    SurfacePrivate(os::ServiceMessageClient<SurfacePrivate>&);
    ~SurfacePrivate();

    static SurfacePrivate& getPrivate(Surface&);
    static void setPrivate(Surface&, std::shared_ptr<SurfacePrivate>&&);

    static void initService();

    void resize(Surface&, int32_t, int32_t);

    void createFromParcel(Parcel&) override;
    void readFromParcel(Parcel& source) override;
    void writeToParcel(Parcel& dest, int32_t flags) const override;

private:
    std::shared_ptr<SurfaceTexture> m_surfaceTexture;
    int32_t m_width;
    int32_t m_height;
    uintptr_t m_shareHandle;
    std::shared_ptr<os::MemoryFilePrivate> m_backbuffer;
};

} // namespace view
} // namespace android
