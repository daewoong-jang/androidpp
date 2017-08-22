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

#include "PlatformMutex.h"

#include "PlatformEvent.h"
#include <android++/LogHelper.h>

namespace android {
namespace os {

intptr_t PlatformMutex::platformCreate()
{
    return reinterpret_cast<intptr_t>(::CreateMutexA(NULL, FALSE, NULL));
}

bool PlatformMutex::platformLock(int64_t milliseconds)
{
    bool lock = false;

    DWORD result = ::WaitForSingleObject(reinterpret_cast<HANDLE>(handle()), milliseconds == PlatformEvent::WAIT_INFINITE ? INFINITE : milliseconds);
    switch (result) {
    case WAIT_OBJECT_0:
        lock = true;
    case WAIT_TIMEOUT:
    case WAIT_ABANDONED:
        break;
    case WAIT_FAILED:
    default:
        LOGE("Failed to lock mutex object");
        break;
    }

    return lock;
}

void PlatformMutex::platformUnlock()
{
    BOOL ok = ::ReleaseMutex(reinterpret_cast<HANDLE>(handle()));
    if (!ok) {
        LOGE("Failed to release mutex object");
    }
}

} // namespace os
} // namespace android
