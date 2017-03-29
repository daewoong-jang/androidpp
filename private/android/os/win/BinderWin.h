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

#include <windows.h>

namespace android {
namespace os {

class BinderWin : public Binder {
public:
    BinderWin(HWND, Client*);
    virtual ~BinderWin();

    intptr_t handle() const override { return reinterpret_cast<intptr_t>(m_hwnd); }
    virtual app::HostWindow* window() override { return nullptr; }

    virtual bool transact(int32_t code, Parcel& data, int32_t flags) override;

protected:
    HWND m_hwnd;
};

class LocalBinderWin : public BinderWin {
public:
    LocalBinderWin(HWND, Client*);
    virtual ~LocalBinderWin();

    bool isLocal() const override { return true; }

    virtual bool transact(int32_t code, Parcel& data, int32_t flags) override;
};

class MessageBinderWin : public LocalBinderWin {
public:
    MessageBinderWin(Client*);
    virtual ~MessageBinderWin();

    bool start();
    bool startAtTime(std::chrono::milliseconds);
    void stop();

    void close();

private:
    static void registerMessageWindowClass();

    static LRESULT CALLBACK messageWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT messageWindowProcInternal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static void NTAPI messageWindowQueueTimerCallback(PVOID parameter, BOOLEAN);

    UINT m_activeTimerID;
    bool m_shouldUseHighResolutionTimers;
    HANDLE m_timerQueue;
    HANDLE m_timerQueueTimer;
    bool m_highResTimerActive;
    bool m_processingCustomTimerMessage;
    LONG m_pendingTimers;
};

} // namespace os
} // namespace android
