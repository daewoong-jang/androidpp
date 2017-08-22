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

#include "PlatformHandle.h"

namespace android {
namespace os {

PlatformHandle::PlatformHandle()
{
}

PlatformHandle::PlatformHandle(intptr_t handle)
    : m_handle(handle)
{
}

PlatformHandle::~PlatformHandle()
{
    close();
}

void PlatformHandle::close()
{
    close(m_handle);
    m_handle = 0;
}

void PlatformHandle::close(intptr_t handle)
{
    platformClose(handle);
}

void PlatformHandle::setHandle(intptr_t handle)
{
    close();
    m_handle = handle;
}

void PlatformHandle::readFromParcel(Parcel& source)
{
    int64_t sourcePid;
    source >> sourcePid;

    intptr_t handle;
    source >> handle;

    m_handle = platformDuplicate(handle, sourcePid);
}

void PlatformHandle::writeToParcel(Parcel& dest, int32_t flags) const
{
    intptr_t handle = platformDuplicate(m_handle);
    dest << System::getProcessId();
    dest << handle;
}

} // namespace os
} // namespace android
