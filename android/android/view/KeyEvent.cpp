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

#include "KeyEvent.h"

#include <android/LazyInitializedPtr.h>
#include <android/view/KeyEventPrivate.h>
#include <android++/LogHelper.h>

namespace android {
namespace view {

KeyEvent::KeyEvent(int32_t action, int32_t code)
{
    m_eventTime = System::currentTimeMillis();
    m_downTime = System::currentTimeMillis();
    m_action = action;
    m_code = code;
}

KeyEvent::KeyEvent(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, int32_t code, int32_t repeat)
{
    m_eventTime = eventTime;
    m_downTime = downTime;
    m_action = action;
    m_code = code;
    m_repeat = repeat;
}

KeyEvent::KeyEvent(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, int32_t code, int32_t repeat, int32_t metaState)
{
    m_eventTime = eventTime;
    m_downTime = downTime;
    m_action = action;
    m_code = code;
    m_repeat = repeat;
    m_metaState = metaState;
}

KeyEvent::KeyEvent(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, int32_t code, int32_t repeat, int32_t metaState, int32_t deviceId, int32_t scancode)
{
    m_eventTime = eventTime;
    m_downTime = downTime;
    m_action = action;
    m_code = code;
    m_repeat = repeat;
    m_metaState = metaState;
    m_scancode = scancode;
}

KeyEvent::KeyEvent(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, int32_t code, int32_t repeat, int32_t metaState, int32_t deviceId, int32_t scancode, int32_t flags)
{
    m_eventTime = eventTime;
    m_downTime = downTime;
    m_action = action;
    m_code = code;
    m_repeat = repeat;
    m_metaState = metaState;
    m_scancode = scancode;
    m_flags = flags;
}

KeyEvent::KeyEvent(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, int32_t code, int32_t repeat, int32_t metaState, int32_t deviceId, int32_t scancode, int32_t flags, int32_t source)
{
    m_eventTime = eventTime;
    m_downTime = downTime;
    m_action = action;
    m_code = code;
    m_repeat = repeat;
    m_metaState = metaState;
    m_scancode = scancode;
    m_flags = flags;
    m_source = source;
}

KeyEvent::KeyEvent(const KeyEvent& origEvent)
    : m_eventTime(origEvent.m_eventTime)
    , m_downTime(origEvent.m_downTime)
    , m_action(origEvent.m_action)
    , m_code(origEvent.m_code)
    , m_repeat(origEvent.m_repeat)
    , m_metaState(origEvent.m_metaState)
    , m_scancode(origEvent.m_scancode)
    , m_flags(origEvent.m_flags)
    , m_source(origEvent.m_source)
    , m_nativeKeyCode(origEvent.m_nativeKeyCode)
    , m_virtualKeyCode(origEvent.m_virtualKeyCode)
    , m_isSystemKey(origEvent.m_isSystemKey)
    , m_character(origEvent.m_character)
{
}

KeyEvent::KeyEvent(KeyEvent&& origEvent)
    : m_eventTime(origEvent.m_eventTime)
    , m_downTime(origEvent.m_downTime)
    , m_action(origEvent.m_action)
    , m_code(origEvent.m_code)
    , m_repeat(origEvent.m_repeat)
    , m_metaState(origEvent.m_metaState)
    , m_scancode(origEvent.m_scancode)
    , m_flags(origEvent.m_flags)
    , m_source(origEvent.m_source)
    , m_nativeKeyCode(origEvent.m_nativeKeyCode)
    , m_virtualKeyCode(origEvent.m_virtualKeyCode)
    , m_isSystemKey(origEvent.m_isSystemKey)
    , m_character(origEvent.m_character)
{
}

int32_t KeyEvent::getAction()
{
    return m_action;
}

std::chrono::milliseconds KeyEvent::getDownTime()
{
    return m_downTime;
}

std::chrono::milliseconds KeyEvent::getEventTime()
{
    return m_eventTime;
}

int32_t KeyEvent::getFlags()
{
    return m_flags;
}

int32_t KeyEvent::getKeyCode()
{
    return m_code;
}

int32_t KeyEvent::getMetaState()
{
    return m_metaState;
}

int32_t KeyEvent::getModifierMetaStateMask()
{
    return META_MODIFIER_MASK;
}

int32_t KeyEvent::getModifiers()
{
    return normalizeMetaState(m_metaState) & META_MODIFIER_MASK;
}

int32_t KeyEvent::getRepeatCount()
{
    return m_repeat;
}

int32_t KeyEvent::getScanCode()
{
    return m_scancode;
}

int32_t KeyEvent::getSource()
{
    return m_source;
}

int32_t KeyEvent::getUnicodeChar(int32_t metaState)
{
    return KeyEventPrivate::getMappedCharacter(*this);
}

int32_t KeyEvent::getUnicodeChar()
{
    return getUnicodeChar(m_metaState);
}

bool KeyEvent::hasModifiers(int32_t modifiers)
{
    return metaStateHasModifiers(m_metaState, modifiers);
}

bool KeyEvent::hasNoModifiers()
{
    return metaStateHasNoModifiers(m_metaState);
}

bool KeyEvent::isAltPressed()
{
    return (m_metaState & META_ALT_ON) != 0;
}

bool KeyEvent::isCanceled()
{
    return (m_flags & FLAG_CANCELED) != 0;
}

bool KeyEvent::isCapsLockOn()
{
    return (m_metaState & META_CAPS_LOCK_ON) != 0;
}

bool KeyEvent::isCtrlPressed()
{
    return (m_metaState & META_CTRL_ON) != 0;
}

bool KeyEvent::isFunctionPressed()
{
    return (m_metaState & META_FUNCTION_ON) != 0;
}

bool KeyEvent::isGamepadButton(int32_t keyCode)
{
    switch (keyCode) {
        case KEYCODE_BUTTON_A:
        case KEYCODE_BUTTON_B:
        case KEYCODE_BUTTON_C:
        case KEYCODE_BUTTON_X:
        case KEYCODE_BUTTON_Y:
        case KEYCODE_BUTTON_Z:
        case KEYCODE_BUTTON_L1:
        case KEYCODE_BUTTON_R1:
        case KEYCODE_BUTTON_L2:
        case KEYCODE_BUTTON_R2:
        case KEYCODE_BUTTON_THUMBL:
        case KEYCODE_BUTTON_THUMBR:
        case KEYCODE_BUTTON_START:
        case KEYCODE_BUTTON_SELECT:
        case KEYCODE_BUTTON_MODE:
        case KEYCODE_BUTTON_1:
        case KEYCODE_BUTTON_2:
        case KEYCODE_BUTTON_3:
        case KEYCODE_BUTTON_4:
        case KEYCODE_BUTTON_5:
        case KEYCODE_BUTTON_6:
        case KEYCODE_BUTTON_7:
        case KEYCODE_BUTTON_8:
        case KEYCODE_BUTTON_9:
        case KEYCODE_BUTTON_10:
        case KEYCODE_BUTTON_11:
        case KEYCODE_BUTTON_12:
        case KEYCODE_BUTTON_13:
        case KEYCODE_BUTTON_14:
        case KEYCODE_BUTTON_15:
        case KEYCODE_BUTTON_16:
            return true;
        default:
            return false;
    }
}

bool KeyEvent::isLongPress()
{
    return (m_flags & FLAG_LONG_PRESS) != 0;
}

bool KeyEvent::isMetaPressed()
{
    return (m_metaState & META_META_ON) != 0;
}

bool KeyEvent::isModifierKey(int32_t keyCode)
{
    switch (keyCode) {
        case KEYCODE_SHIFT_LEFT:
        case KEYCODE_SHIFT_RIGHT:
        case KEYCODE_ALT_LEFT:
        case KEYCODE_ALT_RIGHT:
        case KEYCODE_CTRL_LEFT:
        case KEYCODE_CTRL_RIGHT:
        case KEYCODE_META_LEFT:
        case KEYCODE_META_RIGHT:
        case KEYCODE_SYM:
        case KEYCODE_NUM:
        case KEYCODE_FUNCTION:
            return true;
        default:
            return false;
    }
}

bool KeyEvent::isNumLockOn()
{
    return (m_metaState & META_NUM_LOCK_ON) != 0;
}

bool KeyEvent::isScrollLockOn()
{
    return (m_metaState & META_SCROLL_LOCK_ON) != 0;
}

bool KeyEvent::isShiftPressed()
{
    return (m_metaState & META_SHIFT_ON) != 0;
}

bool KeyEvent::isSymPressed()
{
    return (m_metaState & META_SYM_ON) != 0;
}

bool KeyEvent::isSystem()
{
    return KeyEventPrivate::isSystemKey(*this);
}

bool KeyEvent::isTracking()
{
    return (m_flags & FLAG_TRACKING) != 0;
}

// Symbolic names of all key codes.
static const LazyInitializedPtr<std::unordered_map<int32_t, String>> KEYCODE_SYMBOLIC_NAMES([] {
        auto& names = *new std::unordered_map<int32_t, String>();
        names[KeyEvent::KEYCODE_UNKNOWN] = L"KEYCODE_UNKNOWN";
        names[KeyEvent::KEYCODE_SOFT_LEFT] = L"KEYCODE_SOFT_LEFT";
        names[KeyEvent::KEYCODE_SOFT_RIGHT] = L"KEYCODE_SOFT_RIGHT";
        names[KeyEvent::KEYCODE_HOME] = L"KEYCODE_HOME";
        names[KeyEvent::KEYCODE_BACK] = L"KEYCODE_BACK";
        names[KeyEvent::KEYCODE_CALL] = L"KEYCODE_CALL";
        names[KeyEvent::KEYCODE_ENDCALL] = L"KEYCODE_ENDCALL";
        names[KeyEvent::KEYCODE_0] = L"KEYCODE_0";
        names[KeyEvent::KEYCODE_1] = L"KEYCODE_1";
        names[KeyEvent::KEYCODE_2] = L"KEYCODE_2";
        names[KeyEvent::KEYCODE_3] = L"KEYCODE_3";
        names[KeyEvent::KEYCODE_4] = L"KEYCODE_4";
        names[KeyEvent::KEYCODE_5] = L"KEYCODE_5";
        names[KeyEvent::KEYCODE_6] = L"KEYCODE_6";
        names[KeyEvent::KEYCODE_7] = L"KEYCODE_7";
        names[KeyEvent::KEYCODE_8] = L"KEYCODE_8";
        names[KeyEvent::KEYCODE_9] = L"KEYCODE_9";
        names[KeyEvent::KEYCODE_STAR] = L"KEYCODE_STAR";
        names[KeyEvent::KEYCODE_POUND] = L"KEYCODE_POUND";
        names[KeyEvent::KEYCODE_DPAD_UP] = L"KEYCODE_DPAD_UP";
        names[KeyEvent::KEYCODE_DPAD_DOWN] = L"KEYCODE_DPAD_DOWN";
        names[KeyEvent::KEYCODE_DPAD_LEFT] = L"KEYCODE_DPAD_LEFT";
        names[KeyEvent::KEYCODE_DPAD_RIGHT] = L"KEYCODE_DPAD_RIGHT";
        names[KeyEvent::KEYCODE_DPAD_CENTER] = L"KEYCODE_DPAD_CENTER";
        names[KeyEvent::KEYCODE_VOLUME_UP] = L"KEYCODE_VOLUME_UP";
        names[KeyEvent::KEYCODE_VOLUME_DOWN] = L"KEYCODE_VOLUME_DOWN";
        names[KeyEvent::KEYCODE_POWER] = L"KEYCODE_POWER";
        names[KeyEvent::KEYCODE_CAMERA] = L"KEYCODE_CAMERA";
        names[KeyEvent::KEYCODE_CLEAR] = L"KEYCODE_CLEAR";
        names[KeyEvent::KEYCODE_A] = L"KEYCODE_A";
        names[KeyEvent::KEYCODE_B] = L"KEYCODE_B";
        names[KeyEvent::KEYCODE_C] = L"KEYCODE_C";
        names[KeyEvent::KEYCODE_D] = L"KEYCODE_D";
        names[KeyEvent::KEYCODE_E] = L"KEYCODE_E";
        names[KeyEvent::KEYCODE_F] = L"KEYCODE_F";
        names[KeyEvent::KEYCODE_G] = L"KEYCODE_G";
        names[KeyEvent::KEYCODE_H] = L"KEYCODE_H";
        names[KeyEvent::KEYCODE_I] = L"KEYCODE_I";
        names[KeyEvent::KEYCODE_J] = L"KEYCODE_J";
        names[KeyEvent::KEYCODE_K] = L"KEYCODE_K";
        names[KeyEvent::KEYCODE_L] = L"KEYCODE_L";
        names[KeyEvent::KEYCODE_M] = L"KEYCODE_M";
        names[KeyEvent::KEYCODE_N] = L"KEYCODE_N";
        names[KeyEvent::KEYCODE_O] = L"KEYCODE_O";
        names[KeyEvent::KEYCODE_P] = L"KEYCODE_P";
        names[KeyEvent::KEYCODE_Q] = L"KEYCODE_Q";
        names[KeyEvent::KEYCODE_R] = L"KEYCODE_R";
        names[KeyEvent::KEYCODE_S] = L"KEYCODE_S";
        names[KeyEvent::KEYCODE_T] = L"KEYCODE_T";
        names[KeyEvent::KEYCODE_U] = L"KEYCODE_U";
        names[KeyEvent::KEYCODE_V] = L"KEYCODE_V";
        names[KeyEvent::KEYCODE_W] = L"KEYCODE_W";
        names[KeyEvent::KEYCODE_X] = L"KEYCODE_X";
        names[KeyEvent::KEYCODE_Y] = L"KEYCODE_Y";
        names[KeyEvent::KEYCODE_Z] = L"KEYCODE_Z";
        names[KeyEvent::KEYCODE_COMMA] = L"KEYCODE_COMMA";
        names[KeyEvent::KEYCODE_PERIOD] = L"KEYCODE_PERIOD";
        names[KeyEvent::KEYCODE_ALT_LEFT] = L"KEYCODE_ALT_LEFT";
        names[KeyEvent::KEYCODE_ALT_RIGHT] = L"KEYCODE_ALT_RIGHT";
        names[KeyEvent::KEYCODE_SHIFT_LEFT] = L"KEYCODE_SHIFT_LEFT";
        names[KeyEvent::KEYCODE_SHIFT_RIGHT] = L"KEYCODE_SHIFT_RIGHT";
        names[KeyEvent::KEYCODE_TAB] = L"KEYCODE_TAB";
        names[KeyEvent::KEYCODE_SPACE] = L"KEYCODE_SPACE";
        names[KeyEvent::KEYCODE_SYM] = L"KEYCODE_SYM";
        names[KeyEvent::KEYCODE_EXPLORER] = L"KEYCODE_EXPLORER";
        names[KeyEvent::KEYCODE_ENVELOPE] = L"KEYCODE_ENVELOPE";
        names[KeyEvent::KEYCODE_ENTER] = L"KEYCODE_ENTER";
        names[KeyEvent::KEYCODE_DEL] = L"KEYCODE_DEL";
        names[KeyEvent::KEYCODE_GRAVE] = L"KEYCODE_GRAVE";
        names[KeyEvent::KEYCODE_MINUS] = L"KEYCODE_MINUS";
        names[KeyEvent::KEYCODE_EQUALS] = L"KEYCODE_EQUALS";
        names[KeyEvent::KEYCODE_LEFT_BRACKET] = L"KEYCODE_LEFT_BRACKET";
        names[KeyEvent::KEYCODE_RIGHT_BRACKET] = L"KEYCODE_RIGHT_BRACKET";
        names[KeyEvent::KEYCODE_BACKSLASH] = L"KEYCODE_BACKSLASH";
        names[KeyEvent::KEYCODE_SEMICOLON] = L"KEYCODE_SEMICOLON";
        names[KeyEvent::KEYCODE_APOSTROPHE] = L"KEYCODE_APOSTROPHE";
        names[KeyEvent::KEYCODE_SLASH] = L"KEYCODE_SLASH";
        names[KeyEvent::KEYCODE_AT] = L"KEYCODE_AT";
        names[KeyEvent::KEYCODE_NUM] = L"KEYCODE_NUM";
        names[KeyEvent::KEYCODE_HEADSETHOOK] = L"KEYCODE_HEADSETHOOK";
        names[KeyEvent::KEYCODE_FOCUS] = L"KEYCODE_FOCUS";
        names[KeyEvent::KEYCODE_PLUS] = L"KEYCODE_PLUS";
        names[KeyEvent::KEYCODE_MENU] = L"KEYCODE_MENU";
        names[KeyEvent::KEYCODE_NOTIFICATION] = L"KEYCODE_NOTIFICATION";
        names[KeyEvent::KEYCODE_SEARCH] = L"KEYCODE_SEARCH";
        names[KeyEvent::KEYCODE_MEDIA_PLAY_PAUSE] = L"KEYCODE_MEDIA_PLAY_PAUSE";
        names[KeyEvent::KEYCODE_MEDIA_STOP] = L"KEYCODE_MEDIA_STOP";
        names[KeyEvent::KEYCODE_MEDIA_NEXT] = L"KEYCODE_MEDIA_NEXT";
        names[KeyEvent::KEYCODE_MEDIA_PREVIOUS] = L"KEYCODE_MEDIA_PREVIOUS";
        names[KeyEvent::KEYCODE_MEDIA_REWIND] = L"KEYCODE_MEDIA_REWIND";
        names[KeyEvent::KEYCODE_MEDIA_FAST_FORWARD] = L"KEYCODE_MEDIA_FAST_FORWARD";
        names[KeyEvent::KEYCODE_MUTE] = L"KEYCODE_MUTE";
        names[KeyEvent::KEYCODE_PAGE_UP] = L"KEYCODE_PAGE_UP";
        names[KeyEvent::KEYCODE_PAGE_DOWN] = L"KEYCODE_PAGE_DOWN";
        names[KeyEvent::KEYCODE_PICTSYMBOLS] = L"KEYCODE_PICTSYMBOLS";
        names[KeyEvent::KEYCODE_SWITCH_CHARSET] = L"KEYCODE_SWITCH_CHARSET";
        names[KeyEvent::KEYCODE_BUTTON_A] = L"KEYCODE_BUTTON_A";
        names[KeyEvent::KEYCODE_BUTTON_B] = L"KEYCODE_BUTTON_B";
        names[KeyEvent::KEYCODE_BUTTON_C] = L"KEYCODE_BUTTON_C";
        names[KeyEvent::KEYCODE_BUTTON_X] = L"KEYCODE_BUTTON_X";
        names[KeyEvent::KEYCODE_BUTTON_Y] = L"KEYCODE_BUTTON_Y";
        names[KeyEvent::KEYCODE_BUTTON_Z] = L"KEYCODE_BUTTON_Z";
        names[KeyEvent::KEYCODE_BUTTON_L1] = L"KEYCODE_BUTTON_L1";
        names[KeyEvent::KEYCODE_BUTTON_R1] = L"KEYCODE_BUTTON_R1";
        names[KeyEvent::KEYCODE_BUTTON_L2] = L"KEYCODE_BUTTON_L2";
        names[KeyEvent::KEYCODE_BUTTON_R2] = L"KEYCODE_BUTTON_R2";
        names[KeyEvent::KEYCODE_BUTTON_THUMBL] = L"KEYCODE_BUTTON_THUMBL";
        names[KeyEvent::KEYCODE_BUTTON_THUMBR] = L"KEYCODE_BUTTON_THUMBR";
        names[KeyEvent::KEYCODE_BUTTON_START] = L"KEYCODE_BUTTON_START";
        names[KeyEvent::KEYCODE_BUTTON_SELECT] = L"KEYCODE_BUTTON_SELECT";
        names[KeyEvent::KEYCODE_BUTTON_MODE] = L"KEYCODE_BUTTON_MODE";
        names[KeyEvent::KEYCODE_ESCAPE] = L"KEYCODE_ESCAPE";
        names[KeyEvent::KEYCODE_FORWARD_DEL] = L"KEYCODE_FORWARD_DEL";
        names[KeyEvent::KEYCODE_CTRL_LEFT] = L"KEYCODE_CTRL_LEFT";
        names[KeyEvent::KEYCODE_CTRL_RIGHT] = L"KEYCODE_CTRL_RIGHT";
        names[KeyEvent::KEYCODE_CAPS_LOCK] = L"KEYCODE_CAPS_LOCK";
        names[KeyEvent::KEYCODE_SCROLL_LOCK] = L"KEYCODE_SCROLL_LOCK";
        names[KeyEvent::KEYCODE_META_LEFT] = L"KEYCODE_META_LEFT";
        names[KeyEvent::KEYCODE_META_RIGHT] = L"KEYCODE_META_RIGHT";
        names[KeyEvent::KEYCODE_FUNCTION] = L"KEYCODE_FUNCTION";
        names[KeyEvent::KEYCODE_SYSRQ] = L"KEYCODE_SYSRQ";
        names[KeyEvent::KEYCODE_BREAK] = L"KEYCODE_BREAK";
        names[KeyEvent::KEYCODE_MOVE_HOME] = L"KEYCODE_MOVE_HOME";
        names[KeyEvent::KEYCODE_MOVE_END] = L"KEYCODE_MOVE_END";
        names[KeyEvent::KEYCODE_INSERT] = L"KEYCODE_INSERT";
        names[KeyEvent::KEYCODE_FORWARD] = L"KEYCODE_FORWARD";
        names[KeyEvent::KEYCODE_MEDIA_PLAY] = L"KEYCODE_MEDIA_PLAY";
        names[KeyEvent::KEYCODE_MEDIA_PAUSE] = L"KEYCODE_MEDIA_PAUSE";
        names[KeyEvent::KEYCODE_MEDIA_CLOSE] = L"KEYCODE_MEDIA_CLOSE";
        names[KeyEvent::KEYCODE_MEDIA_EJECT] = L"KEYCODE_MEDIA_EJECT";
        names[KeyEvent::KEYCODE_MEDIA_RECORD] = L"KEYCODE_MEDIA_RECORD";
        names[KeyEvent::KEYCODE_F1] = L"KEYCODE_F1";
        names[KeyEvent::KEYCODE_F2] = L"KEYCODE_F2";
        names[KeyEvent::KEYCODE_F3] = L"KEYCODE_F3";
        names[KeyEvent::KEYCODE_F4] = L"KEYCODE_F4";
        names[KeyEvent::KEYCODE_F5] = L"KEYCODE_F5";
        names[KeyEvent::KEYCODE_F6] = L"KEYCODE_F6";
        names[KeyEvent::KEYCODE_F7] = L"KEYCODE_F7";
        names[KeyEvent::KEYCODE_F8] = L"KEYCODE_F8";
        names[KeyEvent::KEYCODE_F9] = L"KEYCODE_F9";
        names[KeyEvent::KEYCODE_F10] = L"KEYCODE_F10";
        names[KeyEvent::KEYCODE_F11] = L"KEYCODE_F11";
        names[KeyEvent::KEYCODE_F12] = L"KEYCODE_F12";
        names[KeyEvent::KEYCODE_NUM_LOCK] = L"KEYCODE_NUM_LOCK";
        names[KeyEvent::KEYCODE_NUMPAD_0] = L"KEYCODE_NUMPAD_0";
        names[KeyEvent::KEYCODE_NUMPAD_1] = L"KEYCODE_NUMPAD_1";
        names[KeyEvent::KEYCODE_NUMPAD_2] = L"KEYCODE_NUMPAD_2";
        names[KeyEvent::KEYCODE_NUMPAD_3] = L"KEYCODE_NUMPAD_3";
        names[KeyEvent::KEYCODE_NUMPAD_4] = L"KEYCODE_NUMPAD_4";
        names[KeyEvent::KEYCODE_NUMPAD_5] = L"KEYCODE_NUMPAD_5";
        names[KeyEvent::KEYCODE_NUMPAD_6] = L"KEYCODE_NUMPAD_6";
        names[KeyEvent::KEYCODE_NUMPAD_7] = L"KEYCODE_NUMPAD_7";
        names[KeyEvent::KEYCODE_NUMPAD_8] = L"KEYCODE_NUMPAD_8";
        names[KeyEvent::KEYCODE_NUMPAD_9] = L"KEYCODE_NUMPAD_9";
        names[KeyEvent::KEYCODE_NUMPAD_DIVIDE] = L"KEYCODE_NUMPAD_DIVIDE";
        names[KeyEvent::KEYCODE_NUMPAD_MULTIPLY] = L"KEYCODE_NUMPAD_MULTIPLY";
        names[KeyEvent::KEYCODE_NUMPAD_SUBTRACT] = L"KEYCODE_NUMPAD_SUBTRACT";
        names[KeyEvent::KEYCODE_NUMPAD_ADD] = L"KEYCODE_NUMPAD_ADD";
        names[KeyEvent::KEYCODE_NUMPAD_DOT] = L"KEYCODE_NUMPAD_DOT";
        names[KeyEvent::KEYCODE_NUMPAD_COMMA] = L"KEYCODE_NUMPAD_COMMA";
        names[KeyEvent::KEYCODE_NUMPAD_ENTER] = L"KEYCODE_NUMPAD_ENTER";
        names[KeyEvent::KEYCODE_NUMPAD_EQUALS] = L"KEYCODE_NUMPAD_EQUALS";
        names[KeyEvent::KEYCODE_NUMPAD_LEFT_PAREN] = L"KEYCODE_NUMPAD_LEFT_PAREN";
        names[KeyEvent::KEYCODE_NUMPAD_RIGHT_PAREN] = L"KEYCODE_NUMPAD_RIGHT_PAREN";
        names[KeyEvent::KEYCODE_VOLUME_MUTE] = L"KEYCODE_VOLUME_MUTE";
        names[KeyEvent::KEYCODE_INFO] = L"KEYCODE_INFO";
        names[KeyEvent::KEYCODE_CHANNEL_UP] = L"KEYCODE_CHANNEL_UP";
        names[KeyEvent::KEYCODE_CHANNEL_DOWN] = L"KEYCODE_CHANNEL_DOWN";
        names[KeyEvent::KEYCODE_ZOOM_IN] = L"KEYCODE_ZOOM_IN";
        names[KeyEvent::KEYCODE_ZOOM_OUT] = L"KEYCODE_ZOOM_OUT";
        names[KeyEvent::KEYCODE_TV] = L"KEYCODE_TV";
        names[KeyEvent::KEYCODE_WINDOW] = L"KEYCODE_WINDOW";
        names[KeyEvent::KEYCODE_GUIDE] = L"KEYCODE_GUIDE";
        names[KeyEvent::KEYCODE_DVR] = L"KEYCODE_DVR";
        names[KeyEvent::KEYCODE_BOOKMARK] = L"KEYCODE_BOOKMARK";
        names[KeyEvent::KEYCODE_CAPTIONS] = L"KEYCODE_CAPTIONS";
        names[KeyEvent::KEYCODE_SETTINGS] = L"KEYCODE_SETTINGS";
        names[KeyEvent::KEYCODE_TV_POWER] = L"KEYCODE_TV_POWER";
        names[KeyEvent::KEYCODE_TV_INPUT] = L"KEYCODE_TV_INPUT";
        names[KeyEvent::KEYCODE_STB_INPUT] = L"KEYCODE_STB_INPUT";
        names[KeyEvent::KEYCODE_STB_POWER] = L"KEYCODE_STB_POWER";
        names[KeyEvent::KEYCODE_AVR_POWER] = L"KEYCODE_AVR_POWER";
        names[KeyEvent::KEYCODE_AVR_INPUT] = L"KEYCODE_AVR_INPUT";
        names[KeyEvent::KEYCODE_PROG_RED] = L"KEYCODE_PROG_RED";
        names[KeyEvent::KEYCODE_PROG_GREEN] = L"KEYCODE_PROG_GREEN";
        names[KeyEvent::KEYCODE_PROG_YELLOW] = L"KEYCODE_PROG_YELLOW";
        names[KeyEvent::KEYCODE_PROG_BLUE] = L"KEYCODE_PROG_BLUE";
        names[KeyEvent::KEYCODE_APP_SWITCH] = L"KEYCODE_APP_SWITCH";
        names[KeyEvent::KEYCODE_BUTTON_1] = L"KEYCODE_BUTTON_1";
        names[KeyEvent::KEYCODE_BUTTON_2] = L"KEYCODE_BUTTON_2";
        names[KeyEvent::KEYCODE_BUTTON_3] = L"KEYCODE_BUTTON_3";
        names[KeyEvent::KEYCODE_BUTTON_4] = L"KEYCODE_BUTTON_4";
        names[KeyEvent::KEYCODE_BUTTON_5] = L"KEYCODE_BUTTON_5";
        names[KeyEvent::KEYCODE_BUTTON_6] = L"KEYCODE_BUTTON_6";
        names[KeyEvent::KEYCODE_BUTTON_7] = L"KEYCODE_BUTTON_7";
        names[KeyEvent::KEYCODE_BUTTON_8] = L"KEYCODE_BUTTON_8";
        names[KeyEvent::KEYCODE_BUTTON_9] = L"KEYCODE_BUTTON_9";
        names[KeyEvent::KEYCODE_BUTTON_10] = L"KEYCODE_BUTTON_10";
        names[KeyEvent::KEYCODE_BUTTON_11] = L"KEYCODE_BUTTON_11";
        names[KeyEvent::KEYCODE_BUTTON_12] = L"KEYCODE_BUTTON_12";
        names[KeyEvent::KEYCODE_BUTTON_13] = L"KEYCODE_BUTTON_13";
        names[KeyEvent::KEYCODE_BUTTON_14] = L"KEYCODE_BUTTON_14";
        names[KeyEvent::KEYCODE_BUTTON_15] = L"KEYCODE_BUTTON_15";
        names[KeyEvent::KEYCODE_BUTTON_16] = L"KEYCODE_BUTTON_16";
        names[KeyEvent::KEYCODE_LANGUAGE_SWITCH] = L"KEYCODE_LANGUAGE_SWITCH";
        names[KeyEvent::KEYCODE_MANNER_MODE] = L"KEYCODE_MANNER_MODE";
        names[KeyEvent::KEYCODE_3D_MODE] = L"KEYCODE_3D_MODE";
        names[KeyEvent::KEYCODE_CONTACTS] = L"KEYCODE_CONTACTS";
        names[KeyEvent::KEYCODE_CALENDAR] = L"KEYCODE_CALENDAR";
        names[KeyEvent::KEYCODE_MUSIC] = L"KEYCODE_MUSIC";
        names[KeyEvent::KEYCODE_CALCULATOR] = L"KEYCODE_CALCULATOR";
        names[KeyEvent::KEYCODE_ZENKAKU_HANKAKU] = L"KEYCODE_ZENKAKU_HANKAKU";
        names[KeyEvent::KEYCODE_EISU] = L"KEYCODE_EISU";
        names[KeyEvent::KEYCODE_MUHENKAN] = L"KEYCODE_MUHENKAN";
        names[KeyEvent::KEYCODE_HENKAN] = L"KEYCODE_HENKAN";
        names[KeyEvent::KEYCODE_KATAKANA_HIRAGANA] = L"KEYCODE_KATAKANA_HIRAGANA";
        names[KeyEvent::KEYCODE_YEN] = L"KEYCODE_YEN";
        names[KeyEvent::KEYCODE_RO] = L"KEYCODE_RO";
        names[KeyEvent::KEYCODE_KANA] = L"KEYCODE_KANA";
        names[KeyEvent::KEYCODE_ASSIST] = L"KEYCODE_ASSIST";
        return &names;
    }
);

int32_t KeyEvent::keyCodeFromString(StringRef symbolicName)
{
    if (symbolicName.empty()) {
        LOGE("IllegalArgumentException: symbolicName must not be null");
    }

    for (auto& name : *KEYCODE_SYMBOLIC_NAMES) {
        if (symbolicName == name.second) {
            return name.first;
        }
    }

    wchar_t* end;
    return std::wcstol(symbolicName.data(), &end, 10);
}

String KeyEvent::keyCodeToString(int32_t keyCode)
{
    return KEYCODE_SYMBOLIC_NAMES->count(keyCode) > 0 ? (*KEYCODE_SYMBOLIC_NAMES)[keyCode] : std::to_wstring(keyCode);
}

// Symbolic names of all metakeys in bit order from least significant to most significant.
// Accordingly there are exactly 32 values in this table.
static const LazyInitializedPtr<std::vector<std::string>> META_SYMBOLIC_NAMES([] {
    auto& names = *new std::vector<std::string>();
    names.push_back("META_SHIFT_ON");
    names.push_back("META_ALT_ON");
    names.push_back("META_SYM_ON");
    names.push_back("META_FUNCTION_ON");
    names.push_back("META_ALT_LEFT_ON");
    names.push_back("META_ALT_RIGHT_ON");
    names.push_back("META_SHIFT_LEFT_ON");
    names.push_back("META_SHIFT_RIGHT_ON");
    names.push_back("META_CAP_LOCKED");
    names.push_back("META_ALT_LOCKED");
    names.push_back("META_SYM_LOCKED");
    names.push_back("0x00000800");
    names.push_back("META_CTRL_ON");
    names.push_back("META_CTRL_LEFT_ON");
    names.push_back("META_CTRL_RIGHT_ON");
    names.push_back("0x00008000");
    names.push_back("META_META_ON");
    names.push_back("META_META_LEFT_ON");
    names.push_back("META_META_RIGHT_ON");
    names.push_back("0x00080000");
    names.push_back("META_CAPS_LOCK_ON");
    names.push_back("META_NUM_LOCK_ON");
    names.push_back("META_SCROLL_LOCK_ON");
    names.push_back("0x00800000");
    names.push_back("0x01000000");
    names.push_back("0x02000000");
    names.push_back("0x04000000");
    names.push_back("0x08000000");
    names.push_back("0x10000000");
    names.push_back("0x20000000");
    names.push_back("0x40000000");
    names.push_back("0x80000000");
    return &names;
});

static std::string metaStateToString(int32_t metaState)
{
    if (metaState == 0) {
        return "0";
    }
    std::string result;
    int32_t i = 0;
    while (metaState != 0) {
        bool isSet = (metaState & 1) != 0;
        metaState >>= 1;
        if (isSet) {
            std::string name = (*META_SYMBOLIC_NAMES)[i];
            if (result.empty()) {
                if (metaState == 0) {
                    return name;
                }
                result = name;
            } else {
                result.append("|");
                result.append(name);
            }
        }
        i++;
    }
    return result;
}

static int32_t metaStateFilterDirectionalModifiers(int32_t metaState,
        int32_t modifiers, int32_t basic, int32_t left, int32_t right) {
    bool wantBasic = (modifiers & basic) != 0;
    int32_t directional = left | right;
    bool wantLeftOrRight = (modifiers & directional) != 0;

    if (wantBasic) {
        if (wantLeftOrRight) {
            LOGE(("IllegalArgumentException: modifiers must not contain "
                    + metaStateToString(basic) + " combined with "
                    + metaStateToString(left) + " or " + metaStateToString(right))
                .c_str());
        }
        return metaState & ~directional;
    } else if (wantLeftOrRight) {
        return metaState & ~basic;
    } else {
        return metaState;
    }
}

bool KeyEvent::metaStateHasModifiers(int32_t metaState, int32_t modifiers)
{
    if ((modifiers & META_INVALID_MODIFIER_MASK) != 0) {
        LOGE("IllegalArgumentException: modifiers must not contain "
                "META_CAPS_LOCK_ON, META_NUM_LOCK_ON, META_SCROLL_LOCK_ON, "
                "META_CAP_LOCKED, META_ALT_LOCKED, META_SYM_LOCKED, "
                "or META_SELECTING");
    }

    metaState = normalizeMetaState(metaState) & META_MODIFIER_MASK;
    metaState = metaStateFilterDirectionalModifiers(metaState, modifiers,
            META_SHIFT_ON, META_SHIFT_LEFT_ON, META_SHIFT_RIGHT_ON);
    metaState = metaStateFilterDirectionalModifiers(metaState, modifiers,
            META_ALT_ON, META_ALT_LEFT_ON, META_ALT_RIGHT_ON);
    metaState = metaStateFilterDirectionalModifiers(metaState, modifiers,
            META_CTRL_ON, META_CTRL_LEFT_ON, META_CTRL_RIGHT_ON);
    metaState = metaStateFilterDirectionalModifiers(metaState, modifiers,
            META_META_ON, META_META_LEFT_ON, META_META_RIGHT_ON);
    return metaState == modifiers;
}

bool KeyEvent::metaStateHasNoModifiers(int32_t metaState)
{
    return (normalizeMetaState(metaState) & META_MODIFIER_MASK) == 0;
}

int32_t KeyEvent::normalizeMetaState(int32_t metaState)
{
    if ((metaState & (META_SHIFT_LEFT_ON | META_SHIFT_RIGHT_ON)) != 0) {
        metaState |= META_SHIFT_ON;
    }
    if ((metaState & (META_ALT_LEFT_ON | META_ALT_RIGHT_ON)) != 0) {
        metaState |= META_ALT_ON;
    }
    if ((metaState & (META_CTRL_LEFT_ON | META_CTRL_RIGHT_ON)) != 0) {
        metaState |= META_CTRL_ON;
    }
    if ((metaState & (META_META_LEFT_ON | META_META_RIGHT_ON)) != 0) {
        metaState |= META_META_ON;
    }
    if ((metaState & META_CAP_LOCKED) != 0) {
        metaState |= META_CAPS_LOCK_ON;
    }
    if ((metaState & META_ALT_LOCKED) != 0) {
        metaState |= META_ALT_ON;
    }
    if ((metaState & META_SYM_LOCKED) != 0) {
        metaState |= META_SYM_ON;
    }
    return metaState & META_ALL_MASK;
}

void KeyEvent::setSource(int32_t source)
{
    m_source = source;
}

void KeyEvent::startTracking()
{
    m_flags |= FLAG_START_TRACKING;
}

} // namespace view
} // namespace android
