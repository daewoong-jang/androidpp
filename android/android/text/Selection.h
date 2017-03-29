/*
 * Copyright (C) 2017 Daewoong Jang.
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

#include <android/text/Spannable.h>

namespace android {
namespace text {

class Selection {
public:
    ANDROID_EXPORT Selection() = default;
    ANDROID_EXPORT virtual ~Selection() = default;

    // Return the offset of the selection anchor or cursor, or -1 if there is no selection or cursor. 
    ANDROID_EXPORT static int32_t getSelectionStart(CharSequence& text);
    // Return the offset of the selection edge or cursor, or -1 if there is no selection or cursor.
    ANDROID_EXPORT static int32_t getSelectionEnd(CharSequence& text);
    // Move the cursor to offset index.
    ANDROID_EXPORT static void setSelection(Spannable& text, int32_t index);
    // Set the selection anchor to start and the selection edge to stop. 
    ANDROID_EXPORT static void setSelection(Spannable& text, int32_t start, int32_t stop);
};

} // namespace text
} // namespace android

using Selection = android::text::Selection;
