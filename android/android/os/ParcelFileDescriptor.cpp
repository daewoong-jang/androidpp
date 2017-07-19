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

#include "ParcelFileDescriptor.h"

#include <android/os/ParcelablePrivate.h>
#include <android/os/PlatformFileDescriptor.h>

namespace android {
namespace os {

class ParcelFileDescriptorCreator final : public ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        auto keyValue = PlatformFileDescriptor::decode(source);
        auto result = std::make_shared<ParcelFileDescriptor>();
        result->m_fd = std::get<0>(keyValue);
        result->m_handle = std::get<1>(keyValue);
        result->m_sourcePid = std::get<2>(keyValue);
        result->m_close = std::get<3>(keyValue);
        return result;
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    ParcelFileDescriptorCreator()
        : ParcelableCreator(this, L"android.os", L"ParcelFileDescriptor.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> ParcelFileDescriptor::CREATOR([] { return new ParcelFileDescriptorCreator; });

ParcelFileDescriptor::ParcelFileDescriptor()
{
}

ParcelFileDescriptor::~ParcelFileDescriptor()
{
}

std::shared_ptr<ParcelFileDescriptor> ParcelFileDescriptor::adoptFd(int32_t fd)
{
    auto result = std::make_shared<ParcelFileDescriptor>();
    result->m_fd = fd;
    result->m_handle = PlatformFileDescriptor::adoptFd(fd);
    return result;
}

int32_t ParcelFileDescriptor::detachFd()
{
    return PlatformFileDescriptor::detachFd(m_fd, m_handle, m_sourcePid, m_close);
}

int32_t ParcelFileDescriptor::describeContents()
{
    return Parcelable::CONTENTS_FILE_DESCRIPTOR;
}

void ParcelFileDescriptor::writeToParcel(Parcel& dest, int32_t flags) const
{
    ParcelableCreator::writeToParcel(*this, dest);
    PlatformFileDescriptor::encode(dest, m_fd, m_handle, flags);
}

} // namespace os
} // namespace android
