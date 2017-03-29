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

#include "BinderWin.h"

#include <android/os/Messenger.h>
#include <android/os/ParcelPrivate.h>
#include <android++/LogHelper.h>

#include <mmsystem.h>

// Derived from Code from Webkit (https://webkit.org/) under LGPL v2 and BSD licenses (https://webkit.org/licensing-webkit/)

// These aren't in winuser.h with the MSVS 2003 Platform SDK, 
// so use default values in that case.
#ifndef USER_TIMER_MINIMUM
#define USER_TIMER_MINIMUM 0x0000000A
#endif

#ifndef USER_TIMER_MAXIMUM
#define USER_TIMER_MAXIMUM 0x7FFFFFFF
#endif

#ifndef QS_RAWINPUT
#define QS_RAWINPUT         0x0400
#endif

namespace android {
namespace os {

static const LPWSTR kMessageWindowClassName = L"MessageWindow";

const int timerResolution = 1; // To improve timer resolution, we call timeBeginPeriod/timeEndPeriod with this value to increase timer resolution to 1ms.
const int highResolutionThresholdMsec = 16; // Only activate high-res timer for sub-16ms timers (Windows can fire timers at 16ms intervals without changing the system resolution).
const int stopHighResTimerInMsec = 300; // Stop high-res timer after 0.3 seconds to lessen power consumption (we don't use a smaller time since oscillating between high and low resolution breaks timer accuracy on XP).

static UINT timerFiredMessage = 0;

enum {
    sharedTimerID = 1000,
    endHighResTimerID = 1001,
};

std::shared_ptr<Binder> Binder::create(Client& client)
{
    return std::shared_ptr<MessageBinderWin>(new MessageBinderWin(&client), [] (auto) {});
}

std::shared_ptr<Binder> Binder::adopt(intptr_t handle)
{
    return std::make_shared<BinderWin>(reinterpret_cast<HWND>(handle), nullptr);
}

BinderWin::BinderWin(HWND hwnd, Client* client)
    : Binder(client)
    , m_hwnd(hwnd)
{
}

BinderWin::~BinderWin()
{
}

bool BinderWin::transact(int32_t code, Parcel& data, int32_t flags)
{
    if (!IsWindow(m_hwnd)) {
        LOGE("Wrong destination");
        return false;
    }

    COPYDATASTRUCT copyData;
    copyData.dwData = (ULONG_PTR)code;
    copyData.cbData = data.dataSize();
    copyData.lpData = (PVOID)data.data();
    if (!::SendMessageA(m_hwnd, WM_COPYDATA, flags == IBinder::FLAG_ONEWAY ? NULL : (WPARAM)ParcelPrivate::getPrivate(data).getOrigin()->handle(), (LPARAM)&copyData)) {
        DWORD lastError = ::GetLastError();
        LOGE("Transaction failed with error code %d", lastError);
        return false;
    }
    return true;
}

LocalBinderWin::LocalBinderWin(HWND hwnd, Client* client)
    : BinderWin(hwnd, client)
{
}

LocalBinderWin::~LocalBinderWin()
{
}

bool LocalBinderWin::transact(int32_t code, Parcel& data, int32_t flags)
{
    LOGI("Transaction attempt on local binder %x", this);
    m_client->onTransaction(code, data, m_reply, flags);
    return true;
}

MessageBinderWin::MessageBinderWin(Client* client)
    : LocalBinderWin(NULL, client)
    , m_activeTimerID(0)
    , m_shouldUseHighResolutionTimers(true)
    , m_timerQueue(::CreateTimerQueue())
    , m_timerQueueTimer(0)
    , m_highResTimerActive(0)
    , m_processingCustomTimerMessage(false)
    , m_pendingTimers(0)
{
    registerMessageWindowClass();

    m_hwnd = ::CreateWindow(kMessageWindowClassName, 0, 0,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_MESSAGE, 0, 0, this);
}

MessageBinderWin::~MessageBinderWin()
{
}

void MessageBinderWin::registerMessageWindowClass()
{
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [=]{
        WNDCLASS windowClass = { 0 };
        windowClass.lpfnWndProc = MessageBinderWin::messageWindowProc;
        windowClass.cbWndExtra = sizeof(MessageBinderWin*) * 2;
        windowClass.lpszClassName = kMessageWindowClassName;

        timerFiredMessage = ::RegisterWindowMessage(L"HandlerTimerFired");

        if (!::RegisterClass(&windowClass))
            assert(0);
    });
}

#define GWLP_HANDLERPTR(n) ((n) * sizeof(MessageBinderWin*))

LRESULT CALLBACK MessageBinderWin::messageWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (MessageBinderWin* binder = static_cast<MessageBinderWin*>(reinterpret_cast<void*>(::GetWindowLongPtr(hWnd, GWLP_HANDLERPTR(0)))))
        return messageWindowProcInternal(hWnd, message, wParam, lParam);

    if (message == WM_CREATE) {
        LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
        MessageBinderWin* binder = static_cast<MessageBinderWin*>(createStruct->lpCreateParams);
        reinterpret_cast<void*>(::SetWindowLongPtr(hWnd, GWLP_HANDLERPTR(0), reinterpret_cast<LONG_PTR>(binder)));
        binder->m_client->onCreate();
        return 0;
    }

    if (message == WM_CLOSE) {
        MessageBinderWin* binder = static_cast<MessageBinderWin*>(reinterpret_cast<void*>(::GetWindowLongPtr(hWnd, GWLP_HANDLERPTR(1))));
        ::SetWindowLongPtr(hWnd, GWLP_HANDLERPTR(1), 0);
        delete binder;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT MessageBinderWin::messageWindowProcInternal(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    MessageBinderWin* binder = static_cast<MessageBinderWin*>(reinterpret_cast<void*>(::GetWindowLongPtr(hWnd, GWLP_HANDLERPTR(0))));
    switch (message) {
    case WM_COPYDATA: {
        intptr_t replyTo = wParam;
        COPYDATASTRUCT* copyData = (COPYDATASTRUCT*)lParam;
        Parcel data;
        ParcelPrivate::initializeWithCopy(data, reinterpret_cast<int8_t*>(copyData->lpData), copyData->cbData);
        binder->onTransaction(copyData->dwData, data, replyTo, replyTo ? 0 : IBinder::FLAG_ONEWAY);
        return TRUE;
    }
    case WM_TIMER:
        if (wParam == sharedTimerID) {
            ::KillTimer(hWnd, sharedTimerID);
            binder->m_client->onTimer();
        } else if (wParam == endHighResTimerID) {
            ::KillTimer(hWnd, endHighResTimerID);
            binder->m_highResTimerActive = false;
            ::timeEndPeriod(timerResolution);
        }
        return 0;
    default:
        if (message == timerFiredMessage) {
            ::InterlockedExchange(&binder->m_pendingTimers, 0);
            binder->m_processingCustomTimerMessage = true;
            binder->m_client->onTimer();
            binder->m_processingCustomTimerMessage = false;
            return 0;
        }
        break;
    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}

void NTAPI MessageBinderWin::messageWindowQueueTimerCallback(PVOID parameter, BOOLEAN)
{
    MessageBinderWin* binder = static_cast<MessageBinderWin*>(parameter);
    binder->start();
}

bool MessageBinderWin::start()
{
    if (::InterlockedIncrement(&m_pendingTimers) == 1)
        ::PostMessage(m_hwnd, timerFiredMessage, 0, 0);
    return true;
}

bool MessageBinderWin::startAtTime(std::chrono::milliseconds uptimeMillis)
{
    std::chrono::milliseconds delayMillis = uptimeMillis - System::currentTimeMillis();

    DWORD dueTime;
    if (delayMillis.count() > USER_TIMER_MAXIMUM)
        dueTime = USER_TIMER_MAXIMUM;
    else if (delayMillis.count() < 0)
        dueTime = 0;
    else
        dueTime = static_cast<DWORD>(delayMillis.count());

    if (dueTime == 0) {
        start();
        return true;
    }

    bool timerSet = false;

    if (m_shouldUseHighResolutionTimers) {
        if (dueTime < highResolutionThresholdMsec) {
            if (!m_highResTimerActive) {
                m_highResTimerActive = true;
                ::timeBeginPeriod(timerResolution);
            }
            ::SetTimer(m_hwnd, endHighResTimerID, stopHighResTimerInMsec, 0);
        }

        DWORD queueStatus = LOWORD(::GetQueueStatus(QS_PAINT | QS_MOUSEBUTTON | QS_KEY | QS_RAWINPUT));

        // Win32 has a tri-level queue with application messages > user input > WM_PAINT/WM_TIMER.

        // If the queue doesn't contains input events, we use a higher priorty timer event posting mechanism.
        if (!(queueStatus & (QS_MOUSEBUTTON | QS_KEY | QS_RAWINPUT))) {
            if (dueTime < USER_TIMER_MINIMUM && !m_processingCustomTimerMessage && !(queueStatus & QS_PAINT)) {
                // Call PostMessage immediately if the timer is already expired, unless a paint is pending.
                // (we prioritize paints over timers)
                if (::InterlockedIncrement(&m_pendingTimers) == 1)
                    ::PostMessage(m_hwnd, timerFiredMessage, 0, 0);
                timerSet = true;
            } else {
                // Otherwise, delay the PostMessage via a CreateTimerQueueTimer
                if (m_timerQueueTimer)
                    ::DeleteTimerQueueTimer(m_timerQueue, m_timerQueueTimer, 0);
                timerSet = ::CreateTimerQueueTimer(&m_timerQueueTimer, m_timerQueue, messageWindowQueueTimerCallback, this, dueTime, 0, WT_EXECUTEINTIMERTHREAD | WT_EXECUTEONLYONCE);
            }
        }
    }

    if (timerSet) {
        if (m_activeTimerID) {
            ::KillTimer(m_hwnd, m_activeTimerID);
            m_activeTimerID = 0;
        }
    } else {
        m_activeTimerID = ::SetTimer(m_hwnd, sharedTimerID, dueTime, 0);
        m_timerQueueTimer = 0;
    }

    return true;
}

void MessageBinderWin::stop()
{
    if (m_timerQueue && m_timerQueueTimer) {
        ::DeleteTimerQueueTimer(m_timerQueue, m_timerQueueTimer, 0);
        m_timerQueueTimer = 0;
    }

    if (m_activeTimerID) {
        ::KillTimer(m_hwnd, m_activeTimerID);
        m_activeTimerID = 0;
    }
}

void MessageBinderWin::close()
{
    m_client->onDestroy();
    ::SetWindowLongPtr(m_hwnd, GWLP_HANDLERPTR(0), 0);
    ::SetWindowLongPtr(m_hwnd, GWLP_HANDLERPTR(1), reinterpret_cast<LONG_PTR>(this));
    ::PostMessageA(m_hwnd, WM_CLOSE, 0, 0);
}

} // namespace os
} // namespace android
