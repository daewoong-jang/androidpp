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

#include "InputMethodManager.h"

#include <android/app/ActivityHostWindow.h>
#include <android/os/Binder.h>
#include <android/view/ViewPrivate.h>

namespace android {
namespace view {
namespace inputmethod {

bool InputMethodManager::showSoftInput(View& view, int32_t flags)
{
    if (mActive)
        return true;
    mActive = ViewPrivate::getPrivate(view).hostWindow()->startInputMethod(&view);
}

bool InputMethodManager::hideSoftInputFromWindow(std::passed_ptr<IBinder> windowToken, int32_t flags)
{
    if (mActive) {
        app::ActivityHostWindow* window = static_cast<app::ActivityHostWindow*>(std::static_pointer_cast<Binder>(windowToken)->window());
        window->endInputMethod();
    }
    return true;
}

void InputMethodManager::restartInput(View& view)
{
    app::ActivityHostWindow* hostWindow = ViewPrivate::getPrivate(view).hostWindow();
    if (mActive)
        hostWindow->endInputMethod();
    mActive = hostWindow->startInputMethod(&view);
}

void InputMethodManager::updateCursorAnchorInfo(View& view, CursorAnchorInfo& cursorAnchorInfo)
{
}

void InputMethodManager::updateSelection(View& view, int32_t selStart, int32_t selEnd, int32_t candidatesStart, int32_t candidatesEnd)
{
}

} // namespace inputmethod
} // namespace view
} // namespace android
