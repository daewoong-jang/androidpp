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

#include "View.h"

#include <android/app/ActivityHostWindow.h>
#include <android/app/WindowProvider.h>
#include <android/content/ContextWrapper.h>
#include <android/view/ViewPrivate.h>

namespace android {
namespace view {

View::View(Context& context)
    : m_context(context)
    , m_inWindow(false)
    , m_visibility(INVISIBLE)
    , m_hasFocus(false)
    , m_focusable(false)
    , m_private(std::make_unique<ViewPrivate>(*this))
{
}

View::~View()
{
    if (hasFocus())
        m_private->releaseFocus();
}

Context& View::getContext()
{
    return m_context;
}

std::shared_ptr<IBinder> View::getWindowToken()
{
    return m_private->hostWindow()->window()->getWindowToken();
}

void View::getLocationOnScreen(int32_t outLocation[2])
{
    outLocation[0] = 0;
    outLocation[1] = 0;
}

void View::setVisibility(int32_t visibility)
{
    onVisibilityChanged(this, visibility);
}

void View::setFocusable(bool focusable)
{
    if (isFocusable() || focusable == m_focusable)
        return;

    bool focused = hasFocus();

    m_focusable = focusable;

    if (!m_focusable && focused)
        m_private->releaseFocus();
}

bool View::requestFocus()
{
    if (!isFocusable())
        return false;

    m_private->requestFocus();
    return true;
}

void View::invalidate()
{
}

void View::invalidate(Rect&)
{
}

std::shared_ptr<InputConnection> View::onCreateInputConnection(EditorInfo& outAttrs)
{
    return nullptr;
}

bool View::onGenericMotionEvent(MotionEvent&)
{
    return false;
}

bool View::onKeyDown(int32_t, KeyEvent&)
{
    return false;
}

bool View::onKeyLongPress(int32_t, KeyEvent&)
{
    return false;
}

bool View::onKeyMultiple(int32_t, int32_t, KeyEvent&)
{
    return false;
}

bool View::onKeyPreIme(int32_t, KeyEvent&)
{
    return false;
}

bool View::onKeyShortcut(int32_t, KeyEvent&)
{
    return false;
}

bool View::onKeyUp(int32_t, KeyEvent&)
{
    return false;
}

void View::onWindowFocusChanged(bool)
{
}

void View::onAttachedToWindow()
{
    m_inWindow = true;
}

void View::onDetachedFromWindow()
{
    m_inWindow = false;
}

void View::onSizeChanged(int32_t w, int32_t h, int32_t oldw, int32_t oldh)
{
}

void View::onVisibilityChanged(View* changedView, int32_t visibility)
{
    m_visibility = visibility;
}

void View::onWindowVisibilityChanged(int32_t visibility)
{
}

void View::onFocusChanged(bool gainFocus, int32_t direction, Rect& previouslyFocusedRect)
{
    m_hasFocus = gainFocus;

    if (m_focusChangeListener)
        m_focusChangeListener(this, gainFocus);
}

void View::onLayout(bool changed, int32_t left, int32_t top, int32_t right, int32_t bottom)
{
    m_rect = Rect(left, top, right, bottom);
}

void View::onMeasure(int32_t widthMeasureSpec, int32_t heightMeasureSpec)
{
}

} // namespace view
} // namespace android
