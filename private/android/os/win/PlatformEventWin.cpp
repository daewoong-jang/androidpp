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

#include "PlatformEvent.h"

#include <android++/LogHelper.h>

namespace android {
namespace os {

class WaitCallback {
public:
    WaitCallback(PlatformEvent* event);
    ~WaitCallback();

    static VOID CALLBACK fired(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

private:
    PlatformEvent* m_event;
    std::shared_ptr<PlatformEventPrivate> m_private;
};

WaitCallback::WaitCallback(PlatformEvent* event)
    : m_event(event)
    , m_private(event->m_private)
{
}

WaitCallback::~WaitCallback()
{
    assert(!m_private->m_waitHandle);
}

VOID CALLBACK WaitCallback::fired(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
    WaitCallback* callback = static_cast<WaitCallback*>(lpParameter);

    ::UnregisterWait(reinterpret_cast<HANDLE>(callback->m_private->m_waitHandle));
    callback->m_private->m_waitHandle = 0;

    if (callback->m_private->m_destroyed)
        callback->m_event->fired(!!TimerOrWaitFired);

    delete callback;
}

intptr_t PlatformEvent::platformCreate()
{
    return reinterpret_cast<intptr_t>(::CreateEventA(NULL, TRUE, FALSE, NULL));
}

void PlatformEvent::platformWait(int64_t milliseconds, WaitMode waitMode)
{
    assert(handle());
    assert(!m_private->m_waitHandle);

    BOOL ok = FALSE;
    switch (waitMode) {
    case WaitMode::Synchronous: {
        DWORD result = ::WaitForSingleObject(reinterpret_cast<HANDLE>(handle()), milliseconds == WAIT_INFINITE ? INFINITE : milliseconds);
        switch (result) {
        case WAIT_OBJECT_0:
        case WAIT_TIMEOUT:
        case WAIT_ABANDONED:
            ok = true;
            break;
        case WAIT_FAILED:
        default:
            break;
        }
        break;
    }
    case WaitMode::Asynchronous: {
        WaitCallback* callback = new WaitCallback(this);
        ok = ::RegisterWaitForSingleObject(reinterpret_cast<HANDLE*>(&m_private->m_waitHandle), reinterpret_cast<HANDLE>(handle()),
            WaitCallback::fired, callback, milliseconds == WAIT_INFINITE ? INFINITE : milliseconds, WT_EXECUTEDEFAULT);
        break;
    }
    default:
        LOGA("Unknown error");
        return;
    };

    if (!ok) {
        LOGE("Could not wait for event: error=%d", GetLastError());
    }
}

void PlatformEvent::platformNotify()
{
    ::SetEvent(reinterpret_cast<HANDLE>(handle()));
}

} // namespace os
} // namespace android
