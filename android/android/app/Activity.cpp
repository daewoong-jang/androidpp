/*
 * Copyright (C) 2016 Daewoong Jang.
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

#include "Activity.h"

#include <android.h>
#include <android/app/ActivityHostWindow.h>

namespace android {
namespace app {

Activity::Activity(Window topLevelWindow)
    : m_private(new ActivityPrivate(*this))
{
    m_private->initialize(topLevelWindow);
}

Activity::~Activity()
{
}

Intent& Activity::getIntent()
{
    auto& context = static_cast<ApplicationContext&>(getApplicationContext());
    return context.getIntent();
}

Window Activity::getWindow()
{
    return reinterpret_cast<Window>(m_private->hostWindow()->window()->windowHandle());
}

void Activity::onAttachedToWindow()
{
}

void Activity::onBackPressed()
{
}

void Activity::onConfigurationChanged(Configuration& newConfig)
{
}

void Activity::onContentChanged()
{
}

bool Activity::onContextItemSelected(MenuItem& item)
{
    return false;
}

void Activity::onContextMenuClosed(Menu& menu)
{
}

void Activity::onCreateContextMenu(ContextMenu& menu, View& v, ContextMenu::ContextMenuInfo& menuInfo)
{
}

void Activity::onDetachedFromWindow()
{
}

bool Activity::onGenericMotionEvent(MotionEvent& event)
{
    return false;
}

bool Activity::onKeyDown(int32_t keyCode, KeyEvent& event)
{
    return false;
}

bool Activity::onKeyLongPress(int32_t keyCode, KeyEvent& event)
{
    return false;
}

bool Activity::onKeyMultiple(int32_t keyCode, int32_t repeatCount, KeyEvent& event)
{
    return false;
}

bool Activity::onKeyShortcut(int32_t keyCode, KeyEvent& event)
{
    return false;
}

bool Activity::onKeyUp(int32_t keyCode, KeyEvent& event)
{
    return false;
}

void Activity::onLowMemory()
{
}

bool Activity::onMenuItemSelected(int32_t featureId, MenuItem& item)
{
    return false;
}

bool Activity::onMenuOpened(int32_t featureId, Menu& menu)
{
    return false;
}

bool Activity::onTouchEvent(MotionEvent& event)
{
    return false;
}

void Activity::onTrimMemory(int32_t level)
{
}

void Activity::onWindowFocusChanged(bool hasFocus)
{
}

void Activity::setContentView(std::passed_ptr<View> view)
{
    m_private->hostWindow()->setContentView(view);
}

void Activity::onCreate(std::passed_ptr<Bundle> savedInstanceState)
{
}

void Activity::onDestroy()
{
}

void Activity::onPause()
{
}

void Activity::onPostCreate(std::passed_ptr<Bundle> savedInstanceState)
{
}

void Activity::onPostResume()
{
}

void Activity::onRestart()
{
}

void Activity::onRestoreInstanceState(std::passed_ptr<Bundle> savedInstanceState)
{
}

void Activity::onResume()
{
}

void Activity::onSaveInstanceState(std::passed_ptr<Bundle> outState)
{
}

void Activity::onStart()
{
}

void Activity::onStop()
{
}

} // namespace app
} // namespace android
