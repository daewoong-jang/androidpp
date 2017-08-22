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

PlatformEvent::PlatformEvent()
    : m_private(std::make_shared<PlatformEventPrivate>())
{
}

PlatformEvent::~PlatformEvent()
{
    std::lock_guard<std::mutex> locker(m_private->m_lock);
    m_private->m_destroyed = true;
    platformNotify();
}

void PlatformEvent::create()
{
    if (handle())
        return;

    setHandle(platformCreate());
}

void PlatformEvent::wait(WaitMode waitMode)
{
    if (!handle())
        return;

    platformWait(WAIT_INFINITE, waitMode);
}

void PlatformEvent::waitFor(std::chrono::milliseconds time, WaitMode waitMode)
{
    if (!handle())
        return;

    platformWait(time.count(), waitMode);
}

void PlatformEvent::notify()
{
    if (!handle())
        return;

    platformNotify();
}

void PlatformEvent::cancel()
{
    if (!handle())
        return;

    std::lock_guard<std::mutex> locker(m_private->m_lock);
    m_private->m_cancelled = true;
    platformNotify();
}

void PlatformEvent::setWaitCallback(std::function<void (PlatformEvent*)>&& callback)
{
    std::lock_guard<std::mutex> locker(m_private->m_lock);
    m_private->m_waitCallback = std::move(callback);
}

void PlatformEvent::setTimeCallback(std::function<void (PlatformEvent*)>&& callback)
{
    std::lock_guard<std::mutex> locker(m_private->m_lock);
    m_private->m_timeCallback = std::move(callback);
}

void PlatformEvent::readFromParcel(Parcel& source)
{
    PlatformHandle::readFromParcel(source);
}

void PlatformEvent::writeToParcel(Parcel& dest, int32_t flags) const
{
    PlatformHandle::writeToParcel(dest, flags);
}

void PlatformEvent::fired(bool isTimeout)
{
    assert(!m_private->m_waitHandle);

    {
        std::lock_guard<std::mutex> locker(m_private->m_lock);
        if (m_private->m_cancelled) {
            m_private->m_cancelled = false;
            return;
        }
    }

    std::function<void (PlatformEvent*)> callback;

    {
        std::lock_guard<std::mutex> locker(m_private->m_lock);
        callback = isTimeout ? m_private->m_timeCallback :m_private->m_waitCallback;
    }

    if (!callback.target<void (PlatformEvent*)>())
        return;
    callback(this);
}

} // namespace os
} // namespace android
