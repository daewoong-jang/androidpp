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

#pragma once

#include <android/content/ContextThemeWrapper.h>
#include <android/content/Intent.h>
#include <android/content/res/Configuration.h>
#include <android/os/Bundle.h>
#include <android/view/ContextMenu.h>
#include <android/view/KeyEvent.h>
#include <android/view/Menu.h>
#include <android/view/MotionEvent.h>
#include <android/view/Window.h>

namespace android {
namespace app {

class ActivityPrivate;

class Activity : public ContextThemeWrapper {
    NONCOPYABLE(Activity);
    friend class ActivityPrivate;
public:
    ANDROID_EXPORT Activity(Window = nullptr);
    ANDROID_EXPORT virtual ~Activity();

    // Return the intent that started this activity. 
    ANDROID_EXPORT Intent& getIntent();
    // Retrieve the current Window for the activity. 
    ANDROID_EXPORT virtual Window getWindow();

    // Called when the main window associated with the activity has been attached to the window manager.
    ANDROID_EXPORT virtual void onAttachedToWindow();
    // Called when the activity has detected the user's press of the back key.
    ANDROID_EXPORT virtual void onBackPressed();
    // Called by the system when the device configuration changes while your activity is running.
    ANDROID_EXPORT virtual void onConfigurationChanged(Configuration& newConfig);
    // This hook is called whenever the content view of the screen changes (due to a call to Window.setContentView or Window.addContentView).
    ANDROID_EXPORT virtual void onContentChanged();
    // This hook is called whenever an item in a context menu is selected.
    ANDROID_EXPORT virtual bool onContextItemSelected(MenuItem& item);
    // This hook is called whenever the context menu is being closed (either by the user canceling the menu with the back/menu button, or when an item is selected).
    ANDROID_EXPORT virtual void onContextMenuClosed(Menu& menu);
    // Called when a context menu for the view is about to be shown.
    ANDROID_EXPORT virtual void onCreateContextMenu(ContextMenu& menu, View& v, ContextMenu::ContextMenuInfo& menuInfo);
    // Called when the main window associated with the activity has been detached from the window manager.
    ANDROID_EXPORT virtual void onDetachedFromWindow();
    // Called when a generic motion event was not handled by any of the views inside of the activity.
    ANDROID_EXPORT virtual bool onGenericMotionEvent(MotionEvent& event);
    // Called when a key was pressed down and not handled by any of the views inside of the activity.
    ANDROID_EXPORT virtual bool onKeyDown(int32_t keyCode, KeyEvent& event);
    // Default implementation of KeyEvent.Callback.onKeyLongPress(): always returns false (doesn't handle the event).
    ANDROID_EXPORT virtual bool onKeyLongPress(int32_t keyCode, KeyEvent& event);
    // Default implementation of KeyEvent.Callback.onKeyMultiple(): always returns false (doesn't handle the event).
    ANDROID_EXPORT virtual bool onKeyMultiple(int32_t keyCode, int32_t repeatCount, KeyEvent& event);
    // Called when a key shortcut event is not handled by any of the views in the Activity.
    ANDROID_EXPORT virtual bool onKeyShortcut(int32_t keyCode, KeyEvent& event);
    // Called when a key was released and not handled by any of the views inside of the activity.
    ANDROID_EXPORT virtual bool onKeyUp(int32_t keyCode, KeyEvent& event);
    // This is called when the overall system is running low on memory, and actively running processes should trim their memory usage.
    ANDROID_EXPORT virtual void onLowMemory();
    // Default implementation of onMenuItemSelected(int, MenuItem) for activities.
    ANDROID_EXPORT virtual bool onMenuItemSelected(int32_t featureId, MenuItem& item);
    // Called when a panel's menu is opened by the user.
    ANDROID_EXPORT virtual bool onMenuOpened(int32_t featureId, Menu& menu);
    // Called when a touch screen event was not handled by any of the views under it.
    ANDROID_EXPORT virtual bool onTouchEvent(MotionEvent& event);
    // Called when the operating system has determined that it is a good time for a process to trim unneeded memory from its process.
    ANDROID_EXPORT virtual void onTrimMemory(int32_t level);
    // Called when the current Window of the activity gains or loses focus.
    ANDROID_EXPORT virtual void onWindowFocusChanged(bool hasFocus);

    // Set the activity content to an explicit view.
    ANDROID_EXPORT virtual void setContentView(std::passed_ptr<View> view);

protected:
    // Called when the activity is starting.
    ANDROID_EXPORT virtual void onCreate(std::passed_ptr<Bundle> savedInstanceState);
    // Perform any final cleanup before an activity is destroyed.
    ANDROID_EXPORT virtual void onDestroy();
    // Called as part of the activity lifecycle when an activity is going into the background, but has not (yet) been killed.
    ANDROID_EXPORT virtual void onPause();
    // Called when activity start-up is complete (after onStart() and onRestoreInstanceState(Bundle) have been called).
    ANDROID_EXPORT virtual void onPostCreate(std::passed_ptr<Bundle> savedInstanceState);
    // Called when activity resume is complete (after onResume() has been called).
    ANDROID_EXPORT virtual void onPostResume();
    // Called after onStop() when the current activity is being re-displayed to the user (the user has navigated back to it).
    ANDROID_EXPORT virtual void onRestart();
    // This method is called after onStart() when the activity is being re-initialized from a previously saved state, given here in savedInstanceState.
    ANDROID_EXPORT virtual void onRestoreInstanceState(std::passed_ptr<Bundle> savedInstanceState);
    // Called after onRestoreInstanceState(Bundle), onRestart(), or onPause(), for your activity to start interacting with the user.
    ANDROID_EXPORT virtual void onResume();
    // Called to retrieve per-instance state from an activity before being killed so that the state can be restored in onCreate(Bundle) or onRestoreInstanceState(Bundle) (the Bundle populated by this method will be passed to both).
    ANDROID_EXPORT virtual void onSaveInstanceState(std::passed_ptr<Bundle> outState);
    // Called after onCreate(Bundle) ? or after onRestart() when the activity had been stopped, but is now again being displayed to the user.
    ANDROID_EXPORT virtual void onStart();
    // Called when you are no longer visible to the user.
    ANDROID_EXPORT virtual void onStop();

private:
    std::unique_ptr<ActivityPrivate> m_private;
};

} // namespace app
} // namespace android

using Activity = android::app::Activity;
