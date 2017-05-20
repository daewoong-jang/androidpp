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

#include "MotionEvent.h"

#include <android/view/MotionEventPrivate.h>
#include <android++/LogHelper.h>

namespace android {
namespace view {

MotionEvent::MotionEvent()
{
}

MotionEvent::MotionEvent(const MotionEvent& other)
    : m_eventTime(other.m_eventTime)
    , m_downTime(other.m_downTime)
    , m_action(other.m_action)
    , m_location(other.m_location)
    , m_rawLocation(other.m_rawLocation)
    , m_pressure(other.m_pressure)
    , m_size(other.m_size)
    , m_metaState(other.m_metaState)
    , m_xPrecision(other.m_xPrecision)
    , m_yPrecision(other.m_yPrecision)
    , m_flags(other.m_flags)
    , m_source(other.m_source)
    , m_actionButton(other.m_actionButton)
    , m_buttonState(other.m_buttonState)
    , m_repeatCount(other.m_repeatCount)
    , m_wheelDelta(other.m_wheelDelta)
{
}

MotionEvent::MotionEvent(MotionEvent&& other)
    : m_eventTime(other.m_eventTime)
    , m_downTime(other.m_downTime)
    , m_action(other.m_action)
    , m_location(other.m_location)
    , m_rawLocation(other.m_rawLocation)
    , m_pressure(other.m_pressure)
    , m_size(other.m_size)
    , m_metaState(other.m_metaState)
    , m_xPrecision(other.m_xPrecision)
    , m_yPrecision(other.m_yPrecision)
    , m_flags(other.m_flags)
    , m_source(other.m_source)
    , m_actionButton(other.m_actionButton)
    , m_buttonState(other.m_buttonState)
    , m_repeatCount(other.m_repeatCount)
    , m_wheelDelta(other.m_wheelDelta)
{
}

MotionEvent& MotionEvent::operator=(const MotionEvent& other)
{
    m_eventTime = other.m_eventTime;
    m_downTime = other.m_downTime;
    m_action = other.m_action;
    m_location = other.m_location;
    m_rawLocation = other.m_rawLocation;
    m_pressure = other.m_pressure;
    m_size = other.m_size;
    m_metaState = other.m_metaState;
    m_xPrecision = other.m_xPrecision;
    m_yPrecision = other.m_yPrecision;
    m_flags = other.m_flags;
    m_source = other.m_source;
    m_actionButton = other.m_actionButton;
    m_buttonState = other.m_buttonState;
    m_repeatCount = other.m_repeatCount;
    m_wheelDelta = other.m_wheelDelta;
    return *this;
}

MotionEvent& MotionEvent::operator=(MotionEvent&& other)
{
    m_eventTime = other.m_eventTime;
    m_downTime = other.m_downTime;
    m_action = other.m_action;
    m_location = other.m_location;
    m_rawLocation = other.m_rawLocation;
    m_pressure = other.m_pressure;
    m_size = other.m_size;
    m_metaState = other.m_metaState;
    m_xPrecision = other.m_xPrecision;
    m_yPrecision = other.m_yPrecision;
    m_flags = other.m_flags;
    m_source = other.m_source;
    m_actionButton = other.m_actionButton;
    m_buttonState = other.m_buttonState;
    m_repeatCount = other.m_repeatCount;
    m_wheelDelta = other.m_wheelDelta;
    return *this;
}

MotionEvent MotionEvent::obtain(std::chrono::milliseconds downTime, std::chrono::milliseconds eventTime, int32_t action, float x, float y, int32_t metaState)
{
    MotionEvent event;
    event.m_eventTime = eventTime;
    event.m_downTime = downTime;
    event.m_action = action;
    event.m_location = PointF(x, y);
    event.m_rawLocation = PointF(x, y);
    event.m_metaState = metaState;
    return std::move(event);
}

MotionEvent MotionEvent::obtain(const MotionEvent& other)
{
    return MotionEvent(other);
}

String MotionEvent::actionToString(int32_t action)
{
    switch (action) {
    case ACTION_DOWN:
        return L"ACTION_DOWN";
    case ACTION_UP:
        return L"ACTION_UP";
    case ACTION_CANCEL:
        return L"ACTION_CANCEL";
    case ACTION_OUTSIDE:
        return L"ACTION_OUTSIDE";
    case ACTION_MOVE:
        return L"ACTION_MOVE";
    case ACTION_HOVER_MOVE:
        return L"ACTION_HOVER_MOVE";
    case ACTION_SCROLL:
        return L"ACTION_SCROLL";
    case ACTION_HOVER_ENTER:
        return L"ACTION_HOVER_ENTER";
    case ACTION_HOVER_EXIT:
        return L"ACTION_HOVER_EXIT";
    }
    int32_t index = (action & ACTION_POINTER_INDEX_MASK) >> ACTION_POINTER_INDEX_SHIFT;
    switch (action & ACTION_MASK) {
    case ACTION_POINTER_DOWN:
        return L"ACTION_POINTER_DOWN(" + std::to_wstring(index) + L")";
    case ACTION_POINTER_UP:
        return L"ACTION_POINTER_UP(" + std::to_wstring(index) + L")";
    default:
        return std::to_wstring(action);
    }
}

// Symbolic names of all axes.
static const std::lazy_ptr<std::unordered_map<int32_t, String>> AXIS_SYMBOLIC_NAMES([] {
        auto& names = *new std::unordered_map<int32_t, String>();
        names[MotionEvent::AXIS_X] = L"AXIS_X";
        names[MotionEvent::AXIS_Y] = L"AXIS_Y";
        names[MotionEvent::AXIS_PRESSURE] = L"AXIS_PRESSURE";
        names[MotionEvent::AXIS_SIZE] = L"AXIS_SIZE";
        names[MotionEvent::AXIS_TOUCH_MAJOR] = L"AXIS_TOUCH_MAJOR";
        names[MotionEvent::AXIS_TOUCH_MINOR] = L"AXIS_TOUCH_MINOR";
        names[MotionEvent::AXIS_TOOL_MAJOR] = L"AXIS_TOOL_MAJOR";
        names[MotionEvent::AXIS_TOOL_MINOR] = L"AXIS_TOOL_MINOR";
        names[MotionEvent::AXIS_ORIENTATION] = L"AXIS_ORIENTATION";
        names[MotionEvent::AXIS_VSCROLL] = L"AXIS_VSCROLL";
        names[MotionEvent::AXIS_HSCROLL] = L"AXIS_HSCROLL";
        names[MotionEvent::AXIS_Z] = L"AXIS_Z";
        names[MotionEvent::AXIS_RX] = L"AXIS_RX";
        names[MotionEvent::AXIS_RY] = L"AXIS_RY";
        names[MotionEvent::AXIS_RZ] = L"AXIS_RZ";
        names[MotionEvent::AXIS_HAT_X] = L"AXIS_HAT_X";
        names[MotionEvent::AXIS_HAT_Y] = L"AXIS_HAT_Y";
        names[MotionEvent::AXIS_LTRIGGER] = L"AXIS_LTRIGGER";
        names[MotionEvent::AXIS_RTRIGGER] = L"AXIS_RTRIGGER";
        names[MotionEvent::AXIS_THROTTLE] = L"AXIS_THROTTLE";
        names[MotionEvent::AXIS_RUDDER] = L"AXIS_RUDDER";
        names[MotionEvent::AXIS_WHEEL] = L"AXIS_WHEEL";
        names[MotionEvent::AXIS_GAS] = L"AXIS_GAS";
        names[MotionEvent::AXIS_BRAKE] = L"AXIS_BRAKE";
        names[MotionEvent::AXIS_DISTANCE] = L"AXIS_DISTANCE";
        names[MotionEvent::AXIS_TILT] = L"AXIS_TILT";
        names[MotionEvent::AXIS_GENERIC_1] = L"AXIS_GENERIC_1";
        names[MotionEvent::AXIS_GENERIC_2] = L"AXIS_GENERIC_2";
        names[MotionEvent::AXIS_GENERIC_3] = L"AXIS_GENERIC_3";
        names[MotionEvent::AXIS_GENERIC_4] = L"AXIS_GENERIC_4";
        names[MotionEvent::AXIS_GENERIC_5] = L"AXIS_GENERIC_5";
        names[MotionEvent::AXIS_GENERIC_6] = L"AXIS_GENERIC_6";
        names[MotionEvent::AXIS_GENERIC_7] = L"AXIS_GENERIC_7";
        names[MotionEvent::AXIS_GENERIC_8] = L"AXIS_GENERIC_8";
        names[MotionEvent::AXIS_GENERIC_9] = L"AXIS_GENERIC_9";
        names[MotionEvent::AXIS_GENERIC_10] = L"AXIS_GENERIC_10";
        names[MotionEvent::AXIS_GENERIC_11] = L"AXIS_GENERIC_11";
        names[MotionEvent::AXIS_GENERIC_12] = L"AXIS_GENERIC_12";
        names[MotionEvent::AXIS_GENERIC_13] = L"AXIS_GENERIC_13";
        names[MotionEvent::AXIS_GENERIC_14] = L"AXIS_GENERIC_14";
        names[MotionEvent::AXIS_GENERIC_15] = L"AXIS_GENERIC_15";
        names[MotionEvent::AXIS_GENERIC_16] = L"AXIS_GENERIC_16";
        return &names;
    }
);

int32_t MotionEvent::axisFromString(StringRef symbolicName)
{
    if (symbolicName.empty()) {
        LOGE("IllegalArgumentException: symbolicName must not be null");
    }

    for (auto& name : *AXIS_SYMBOLIC_NAMES) {
        if (symbolicName == name.second) {
            return name.first;
        }
    }

    wchar_t* end;
    return std::wcstol(symbolicName.data(), &end, 10);
}

String MotionEvent::axisToString(int32_t axis)
{
    return AXIS_SYMBOLIC_NAMES->count(axis) > 0 ? (*AXIS_SYMBOLIC_NAMES)[axis] : std::to_wstring(axis);
}

int32_t MotionEvent::getAction()
{
    return m_action;
}

int32_t MotionEvent::getActionButton()
{
    return m_actionButton;
}

int32_t MotionEvent::getActionMasked()
{
    return m_action & ACTION_MASK;
}

float MotionEvent::getAxisValue(int32_t axis, int32_t pointerIndex)
{
    return MotionEventPrivate::getAxisValue(*this, axis, pointerIndex);
}

float MotionEvent::getAxisValue(int32_t axis)
{
    return MotionEventPrivate::getAxisValue(*this, axis, 0);
}

int32_t MotionEvent::getButtonState()
{
    return m_buttonState;
}

int32_t MotionEvent::getDeviceId()
{
    return 1;
}

std::chrono::milliseconds MotionEvent::getDownTime()
{
    return m_downTime;
}

std::chrono::milliseconds MotionEvent::getEventTime()
{
    return m_eventTime;
}

int32_t MotionEvent::getFlags()
{
    return m_flags;
}

int32_t MotionEvent::getMetaState()
{
    return m_metaState;
}

float MotionEvent::getOrientation()
{
    return MotionEventPrivate::getOrientation(*this, 0);
}

float MotionEvent::getOrientation(int32_t pointerIndex)
{
    return MotionEventPrivate::getOrientation(*this, pointerIndex);
}

int32_t MotionEvent::getPointerCount()
{
    return 1;
}

float MotionEvent::getPressure(int32_t pointerIndex)
{
    return m_pressure;
}

float MotionEvent::getPressure()
{
    return m_pressure;
}

float MotionEvent::getRawX()
{
    return m_rawLocation.x;
}

float MotionEvent::getRawY()
{
    return m_rawLocation.y;
}

float MotionEvent::getSize(int32_t pointerIndex)
{
    return m_size;
}

float MotionEvent::getSize()
{
    return m_size;
}

int32_t MotionEvent::getSource()
{
    return m_source;
}

float MotionEvent::getX(int32_t pointerIndex)
{
    return m_location.x;
}

float MotionEvent::getX()
{
    return m_location.x;
}

float MotionEvent::getXPrecision()
{
    return m_xPrecision;
}

float MotionEvent::getY()
{
    return m_location.y;
}

float MotionEvent::getY(int32_t pointerIndex)
{
    return m_location.y;
}

float MotionEvent::getYPrecision()
{
    return m_yPrecision;
}

bool MotionEvent::isButtonPressed(int32_t button)
{
    return button & m_buttonState;
}

void MotionEvent::offsetLocation(float deltaX, float deltaY)
{
    if (deltaX != 0.0f || deltaY != 0.0f) {
        m_location.offset(deltaX, deltaY);
        m_rawLocation.offset(deltaX, deltaY);
    }
}

void MotionEvent::recycle()
{
    // Does nothing, exists for compatibility.
}

void MotionEvent::setAction(int32_t action)
{
    m_action = action;
}

void MotionEvent::setLocation(float x, float y)
{
    float oldX = getX();
    float oldY = getY();
    offsetLocation(x - oldX, y - oldY);
}

void MotionEvent::setSource(int32_t source)
{
    m_source = source;
}

} // namespace view
} // namespace android
