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

#include <java/lang.h>

namespace android {
namespace view {

class InputDevice final {
public:
    // The input source is unknown.
    static const int32_t SOURCE_UNKNOWN = 0;
    // The input source is a keyboard.
    static const int32_t SOURCE_KEYBOARD = 0x00000101;
    // The input source is a game pad.
    static const int32_t SOURCE_GAMEPAD = 0x00000401;
    // The input source is a touch screen pointing device.
    static const int32_t SOURCE_TOUCHSCREEN = 0x00001002;
    // The input source is a mouse pointing device.
    static const int32_t SOURCE_MOUSE = 0x00002002;
    // The input source is a trackball.
    static const int32_t SOURCE_TRACKBALL = 0x00100004;
    // The input source is a touch pad or digitizer tablet that is not associated with a display (unlike SOURCE_TOUCHSCREEN).
    static const int32_t SOURCE_TOUCHPAD = 0x00100008;
    // The input source is a joystick.
    static const int32_t SOURCE_JOYSTICK = 0x01000010;
};

} // namespace view
} // namespace android

using InputDevice = android::view::InputDevice;
