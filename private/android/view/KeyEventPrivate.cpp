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

#include "KeyEventPrivate.h"

namespace android {
namespace view {

int32_t KeyEventPrivate::getNativeKeyCode(KeyEvent& event)
{
    return event.m_nativeKeyCode;
}

void KeyEventPrivate::setNativeKeyCode(KeyEvent& event, int32_t code)
{
    event.m_nativeKeyCode = code;
}

int32_t KeyEventPrivate::getVirtualKeyCode(KeyEvent& event)
{
    return event.m_virtualKeyCode;
}

void KeyEventPrivate::setVirtualKeyCode(KeyEvent& event, int32_t code)
{
    event.m_virtualKeyCode = code;
}

bool KeyEventPrivate::isSystemKey(KeyEvent& event)
{
    return event.m_isSystemKey;
}

void KeyEventPrivate::setSystemKey(KeyEvent& event, bool value)
{
    event.m_isSystemKey = value;
}

wchar_t KeyEventPrivate::getMappedCharacter(KeyEvent& event)
{
    return event.m_character;
}

void KeyEventPrivate::setMappedCharacter(KeyEvent& event, wchar_t ch)
{
    event.m_character = ch;
}

} // namespace view
} // namespace android
