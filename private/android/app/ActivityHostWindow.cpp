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

#include "ActivityHostWindow.h"

#include <android/app/WindowProvider.h>
#include <android/content/ContextPrivate.h>
#include <android/view/InputDevice.h>
#include <android/view/ViewGroup.h>
#include <android/view/ViewPrivate.h>

#include <algorithm>

namespace android {
namespace app {

class DecorViewPrivate : public view::ViewPrivate {
public:
    DecorViewPrivate(View& view, ActivityHostWindow& window)
        : view::ViewPrivate(view), m_window(window) { }
    ~DecorViewPrivate() = default;

    ActivityHostWindow* hostWindow() const override { return &m_window; }

private:
    ActivityHostWindow& m_window;
};

class ActivityHostWindow::DecorView : public view::ViewGroup {
public:
    DecorView(ActivityHostWindow& window)
        : ViewGroup(content::ContextPrivate::getGlobalContext())
    {
        view::ViewPrivate::setPrivate(*this, std::make_unique<DecorViewPrivate>(*this, window));
    }
    ~DecorView() = default;

    view::View* view() const { return m_view.get(); }
    void setView(std::passed_ptr<view::View> view);

private:
    std::shared_ptr<view::View> m_view;
};

void ActivityHostWindow::DecorView::setView(std::passed_ptr<view::View> view)
{
    if (m_view)
        removeView(m_view);

    m_view = view;

    if (m_view)
        addView(m_view);
}

std::unique_ptr<ActivityHostWindow> ActivityHostWindow::create(Activity& activity, WindowHandle parentWindow, const Rect& clientRect)
{
    return std::unique_ptr<ActivityHostWindow>(new ActivityHostWindow(activity, parentWindow, clientRect));
}

ActivityHostWindow::ActivityHostWindow(Activity& activity, WindowHandle parentWindow, const Rect& clientRect)
    : m_activity(activity)
    , m_handler(std::make_shared<Handler>())
    , m_width(0)
    , m_height(0)
    , m_state(Init)
    , m_isVisible(false)
    , m_isGone(false)
    , m_isFocused(false)
    , m_isAttached(false)
    , m_focusedView(nullptr)
    , m_decorView(std::make_unique<DecorView>(*this))
{
    m_handler->post([=] {
        m_windowProvider = WindowProvider::create(parentWindow, clientRect, *this);
    });
}

ActivityHostWindow::~ActivityHostWindow()
{
}

ActivityPrivate& ActivityHostWindow::activity()
{
    return ActivityPrivate::getPrivate(m_activity);
}

View* ActivityHostWindow::contentView() const
{
    return m_decorView->view();
}

void ActivityHostWindow::windowCreated()
{
    assert(m_state == Init);

    m_state = Create;

    activity().callOnCreate();
    activity().callOnRestoreInstanceState();
}

void ActivityHostWindow::windowDestroyed()
{
    m_decorView->setVisibility(View::GONE);
    m_decorView->onWindowFocusChanged(false);

    if (m_state == Pause) {
        activity().callOnStop();
        m_state = Stop;
    }

    activity().callOnDestroy();

    m_decorView->onDetachedFromWindow();
    m_activity.onDetachedFromWindow();
}

void ActivityHostWindow::windowPositionChanged(int32_t x, int32_t y)
{
}

void ActivityHostWindow::windowSizeChanged(int32_t newWidth, int32_t newHeight, Resize resize)
{
    if (!m_isAttached) {
        m_width = newWidth;
        m_height = newHeight;
        return;
    }

    if (m_width == newWidth && m_height == newHeight)
        return;

    int32_t oldWidth = m_width;
    int32_t oldHeight = m_height;
    m_width = newWidth;
    m_height = newHeight;

    bool windowMinimized = newWidth == 0 && newHeight == 0;
    if (windowMinimized || resize == Minimized) {
        windowIsGone();
        return;
    } else if (m_isGone) {
        windowRestored();
        return;
    }
    
    bool changed = m_decorView->getWidth() != newWidth || m_decorView->getHeight() != newHeight;
    if (!changed)
        return;
    
    m_decorView->onMeasure(newWidth, newHeight);
    m_decorView->onSizeChanged(newWidth, newHeight, oldWidth, oldHeight);
    m_decorView->onLayout(true, 0, 0, newWidth, newHeight);
    invalidate();
}

void ActivityHostWindow::windowFocused(bool focus)
{
    if (!m_isAttached)
        return;

    if (focus == m_isFocused)
        return;

    m_isFocused = focus;

    m_decorView->onWindowFocusChanged(focus);
}

void ActivityHostWindow::windowActivated(bool active, bool stop)
{
    if (active) {
        if (m_state == Stop) {
            activity().callOnRestart();
            m_state = Restart;
        }

        bool isNewWindow = m_state == Create;
        if (m_state == Create || m_state == Restart) {
            activity().callOnStart();
            if (m_state == Create)
                activity().callOnPostCreate();
            m_state = Start;
        }

        if (m_state == Start || m_state == Pause) {
            activity().callOnResume();
            activity().callOnPostResume();
            if (m_isGone)
                m_decorView->setVisibility(View::VISIBLE);
            m_state = Resume;
        }

        if (isNewWindow) {
            m_decorView->setVisibility(View::INVISIBLE);
            m_handler->post([=] {
                attachActivity();
            });
        }
    } else {
        if (stop && m_state == Resume) {
            activity().callOnPause();
            m_state = Pause;
        }
        if (stop && m_state == Pause) {
            activity().callOnSaveInstanceState();
            activity().callOnStop();
            m_state = Stop;
            if (m_isGone)
                m_decorView->setVisibility(View::INVISIBLE);
            m_activity.onTrimMemory(40); // TRIM_MEMORY_BACKGROUND
        }
    }
}

void ActivityHostWindow::windowIsVisible(bool visible)
{
    if (!m_isAttached)
        return;

    if (visible == m_isVisible)
        return;

    m_isVisible = visible;

    if (!m_isVisible) {
        m_decorView->setVisibility(View::INVISIBLE);
        return;
    } else if (!m_isGone) {
        m_decorView->setVisibility(View::VISIBLE);
    }
}

void ActivityHostWindow::dispatchKeyEvent(KeyEvent& event)
{
    View* eventTarget = contentView();
    if (eventTarget) {
        switch (event.getAction()) {
        case KeyEvent::ACTION_DOWN:
            if (eventTarget->onKeyDown(event.getKeyCode(), event))
                return;
            break;
        case KeyEvent::ACTION_UP:
            if (eventTarget->onKeyUp(event.getKeyCode(), event))
                return;
            break;
        default:
            return;
        }
    }

    if (inputConnection())
        if (inputConnection()->sendKeyEvent(event))
            return;

    switch (event.getAction()) {
    case KeyEvent::ACTION_DOWN:
        if (activity().callOnKeyDown(event.getKeyCode(), event))
            return;
        break;
    case KeyEvent::ACTION_UP:
        if (activity().callOnKeyUp(event.getKeyCode(), event))
            return;
        break;
    default:
        return;
    }
}

void ActivityHostWindow::dispatchMouseEvent(MotionEvent& event)
{
    assert(event.getSource() == InputDevice::SOURCE_MOUSE);

    View* eventTarget = contentView();
    if (eventTarget) {
        switch (event.getAction()) {
        case MotionEvent::ACTION_DOWN:
            if (!event.getActionButton() == MotionEvent::BUTTON_SECONDARY)
                m_windowProvider->setWindowFocus(true);
            m_windowProvider->setWindowCaptureMouse(true);
            if (eventTarget->onGenericMotionEvent(event))
                return;
            break;
        case MotionEvent::ACTION_UP:
            m_windowProvider->setWindowCaptureMouse(false);
            if (eventTarget->onGenericMotionEvent(event))
                return;
            break;
        default:
            if (eventTarget->onGenericMotionEvent(event))
                return;
            break;
        }
    }

    if (activity().callOnGenericMotionEvent(event))
        return;
}

WindowProvider* ActivityHostWindow::window() const
{
    return m_windowProvider.get();
}

void ActivityHostWindow::setContentView(std::passed_ptr<View> view)
{
    if (m_isVisible && m_isAttached) {
        m_decorView->onWindowVisibilityChanged(View::GONE);
        m_decorView->onDetachedFromWindow();
    }

    m_decorView->setView(view);

    if (m_isVisible && m_isAttached) {
        m_decorView->onAttachedToWindow();
        m_decorView->onWindowVisibilityChanged(View::VISIBLE);
    }

    activity().callOnContentChanged();

    if (m_isVisible && m_isAttached) {
        m_decorView->onMeasure(m_width, m_height);
        m_decorView->onSizeChanged(m_width, m_height, 0, 0);
        m_decorView->onLayout(true, 0, 0, m_width, m_height);
        invalidate();
    }
}

void ActivityHostWindow::invalidate()
{
    m_decorView->invalidate();
}

void ActivityHostWindow::invalidate(Rect& rect)
{
    m_decorView->invalidate(rect);
}

bool ActivityHostWindow::startInputMethod(View* view)
{
    assert(contentView());

    m_inputConnection = view->onCreateInputConnection(EditorInfo());
    m_windowProvider->startInputMethod(true);
    return true;
}

void ActivityHostWindow::endInputMethod()
{
    m_windowProvider->resetInputMethod();
    m_inputConnection.reset();
    m_windowProvider->startInputMethod(false);
}

void ActivityHostWindow::requestFocus(View* view)
{
    if (view == m_focusedView)
        return;

    if (m_focusedView)
        m_focusedView->onFocusChanged(false, 0, Rect());

    m_focusedView = view;

    if (m_focusedView)
        m_focusedView->onFocusChanged(true, 0, Rect());
}

void ActivityHostWindow::releaseFocus(View* view)
{
    if (view != m_focusedView)
        return;

    if (m_focusedView)
        m_focusedView->onFocusChanged(false, 0, Rect());

    m_focusedView = nullptr;
}

void ActivityHostWindow::windowRestored()
{
    assert(m_isGone);

    m_isGone = false;

    m_decorView->onWindowVisibilityChanged(View::VISIBLE);
}

void ActivityHostWindow::windowIsGone()
{
    assert(!m_isGone);

    m_isGone = true;

    m_decorView->onWindowVisibilityChanged(View::GONE);
}

void ActivityHostWindow::attachActivity()
{
    m_isAttached = true;
    windowIsVisible(true);
    m_activity.onAttachedToWindow();
    m_decorView->onAttachedToWindow();
    m_decorView->onWindowVisibilityChanged(View::VISIBLE);
    m_decorView->onMeasure(m_width, m_height);
    m_decorView->onSizeChanged(m_width, m_height, 0, 0);
    m_decorView->onLayout(true, 0, 0, m_width, m_height);
    windowFocused(true);
    invalidate();
}

void ActivityHostWindow::setCursor(std::passed_ptr<view::Cursor> cursor)
{
    m_windowProvider->setCursor(cursor);
}

class WindowPositionChangeScope {
public:
    WindowPositionChangeScope(ActivityHostWindow::OnWindowPositionChangeListener& listener)
        : m_listener(listener)
    {
        m_listener(true);
    }
    ~WindowPositionChangeScope()
    {
        m_listener(false);
    }

private:
    ActivityHostWindow::OnWindowPositionChangeListener& m_listener;
};

void ActivityHostWindow::sendWindowMove()
{
    m_windowProvider->sendWindowMove();
}

void ActivityHostWindow::setWindowPosition(int32_t x, int32_t y, int32_t width, int32_t height)
{
    WindowPositionChangeScope scope(m_windowPositionChangeListener);
    m_windowProvider->setWindowPosition(x, y, width, height);
}

void ActivityHostWindow::setOnWindowPositionChangeListener(OnWindowPositionChangeListener listener)
{
    m_windowPositionChangeListener = std::move(listener);
}

float ActivityHostWindow::deviceScaleFactor() const
{
    return m_windowProvider->deviceScaleFactor();
}

void ActivityHostWindow::dpiChanged(int32_t dpi)
{
    Configuration config;
    config.densityDpi = dpi;
    m_decorView->onConfigurationChanged(config);
}

} // namespace app
} // namespace android
