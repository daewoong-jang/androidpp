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

#include "PlatformEventPrivate.h"
#include <android++/Noncopyable.h>

namespace android {
namespace os {

class PlatformEvent final : public PlatformHandle {
    friend class PlatformEventPrivate;
    friend class WaitCallback;
    NONCOPYABLE(PlatformEvent);
public:
    static const int64_t WAIT_INFINITE = -1;

    enum class WaitMode { Synchronous, Asynchronous };

    PlatformEvent();
    virtual ~PlatformEvent();

    void create();

    void wait(WaitMode = WaitMode::Synchronous);
    void waitFor(std::chrono::milliseconds, WaitMode = WaitMode::Synchronous);
    void notify();
    void cancel();

    void setWaitCallback(std::function<void (PlatformEvent*)>&&);
    void setTimeCallback(std::function<void (PlatformEvent*)>&&);

    virtual void readFromParcel(Parcel&);
    virtual void writeToParcel(Parcel&, int32_t flags) const;

private:
    void fired(bool isTimeout);

    intptr_t platformCreate();
    void platformWait(int64_t, WaitMode);
    void platformNotify();

    std::shared_ptr<PlatformEventPrivate> m_private;
};

} // namespace os
} // namespace android
