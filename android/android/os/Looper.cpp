/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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

#include "Looper.h"

namespace android {
namespace os {

Looper::Looper()
{
}

Looper::~Looper()
{
}

static Looper* mainLooper;

Looper* Looper::getMainLooper()
{
    if (!mainLooper)
        assert(0);

    return mainLooper;
}

static thread_local Looper* threadLooper;

Looper* Looper::myLooper()
{
    return threadLooper;
}

void Looper::prepareMainLooper()
{
    if (mainLooper)
        return;

    prepare();

    mainLooper = threadLooper;

    platformLooperPrepareMain();
}

void Looper::prepare()
{
    if (myLooper())
        return;

    threadLooper = new Looper();
    threadLooper->m_tid = platformGetThreadId();
}

void Looper::loop()
{
    assert(threadLooper);

    platformLooperLoop();

    delete threadLooper;
}

void Looper::quit()
{
    platformLooperQuit(m_tid, 0);
}

void Looper::quitSafely()
{
    platformLooperQuit(m_tid, 0);
}

} // namespace os
} // namespace android
