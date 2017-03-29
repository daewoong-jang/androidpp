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

#pragma once

#include "HostWindow.h"
#include <android/app/ActivityPrivate.h>
#include <android/graphics/Point.h>
#include <android/os/Handler.h>
#include <android/view/Cursor.h>
#include <android/view/View.h>
#include <android++/Functional.h>

namespace android {
namespace app {

class ActivityHostWindow final : public HostWindow {
public:
    static std::unique_ptr<ActivityHostWindow> create(Activity&, WindowHandle parentWindow, const Rect& clientRect);
    ~ActivityHostWindow();

    typedef std::function<void (bool)> OnWindowPositionChangeListener;

    void windowCreated() override;
    void windowDestroyed() override;

    void windowPositionChanged(int32_t, int32_t) override;
    void windowSizeChanged(int32_t, int32_t, Resize) override;
    void windowFocused(bool) override;
    void windowActivated(bool, bool) override;
    void windowIsVisible(bool) override;

    void dispatchKeyEvent(KeyEvent&) override;
    void dispatchMouseEvent(MotionEvent&) override;

    WindowProvider* window() const override;

    void setContentView(std::passed_ptr<View>);

    void invalidate() override;
    void invalidate(Rect&) override;

    bool startInputMethod(View*);
    void endInputMethod();
    InputConnection* inputConnection() const override { return m_inputConnection.get(); }

    void requestFocus(View*);
    void releaseFocus(View*);

    void setCursor(std::passed_ptr<view::Cursor>);

    void sendWindowMove();
    void setWindowPosition(int32_t x, int32_t y, int32_t width, int32_t height);
    void setOnWindowPositionChangeListener(OnWindowPositionChangeListener listener);

    float deviceScaleFactor() const;
    void dpiChanged(int32_t dpi) override;

private:
    ActivityHostWindow(Activity&, WindowHandle parentWindow, const Rect& clientRect);

    ActivityPrivate& activity();
    View* contentView() const;

    void windowRestored();
    void windowIsGone();
    void attachActivity();

    Activity& m_activity;
    std::shared_ptr<os::Handler> m_handler;
    std::unique_ptr<WindowProvider> m_windowProvider;
    int32_t m_width;
    int32_t m_height;
    State m_state;
    bool m_isVisible;
    bool m_isGone;
    bool m_isFocused;
    bool m_isAttached;
    View* m_focusedView;
    std::shared_ptr<InputConnection> m_inputConnection;

    class DecorView;
    std::unique_ptr<DecorView> m_decorView;
    OnWindowPositionChangeListener m_windowPositionChangeListener;
};

} // namespace app
} // namespace android
