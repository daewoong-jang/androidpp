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

#include "Surface.h"

#include <android/os/ParcelablePrivate.h>
#include <android/os/ServiceMessageHost.h>
#include <android/view/SurfacePrivate.h>
#include <android++/LogHelper.h>

namespace android {
namespace view {

Surface::Surface()
    : m_private(os::ServiceObject::create<SurfacePrivate>())
{
}

Surface::Surface(std::passed_ptr<SurfaceTexture> surfaceTexture)
    : m_private(os::ServiceObject::create<SurfacePrivate>())
{
}

Surface::Surface(UninitializedTag)
{
}

Surface::Surface(const Surface& other)
    : m_private(other.m_private)
{
}

Surface::Surface(Surface&& other)
    : m_private(std::move(other.m_private))
{
}

Surface& Surface::operator=(const Surface& other)
{
    m_private = other.m_private;
    return *this;
}

Surface& Surface::operator=(Surface&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

Surface::~Surface()
{
}

bool Surface::isValid()
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
    return false;
}

void Surface::release()
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void Surface::readFromParcel(Parcel& source)
{
    m_private->readFromParcel(source);
}

class SurfaceCreator final : public os::ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        std::shared_ptr<Surface> result(new Surface(Surface::Uninitialized));
        result->m_private = os::ServiceObject::import<SurfacePrivate>(source);
        return result;
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    SurfaceCreator()
        : ParcelableCreator(this, L"android.view", L"Surface.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> Surface::CREATOR([] { return new SurfaceCreator; });

int32_t Surface::describeContents()
{
    return 0;
}

void Surface::writeToParcel(Parcel& dest, int32_t flags) const
{
    ParcelableCreator::writeToParcel(*this, dest);
    m_private->writeToParcel(dest, flags);
}

} // namespace view
} // namespace android
