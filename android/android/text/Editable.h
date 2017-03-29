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

class Editable : public Spannable {
    NONCOPYABLE(Editable);
public:
    class Factory {
    public:
        // Returns the standard Editable Factory.
        static Factory& getInstance();
        // Returns a new SpannedStringBuilder from the specified CharSequence. 
        std::shared_ptr<Editable> newEditable(CharSequence& source);
    };
    friend class Factory;

    ANDROID_EXPORT Editable() = default;
    ANDROID_EXPORT virtual ~Editable() = default;
    ANDROID_EXPORT operator CharSequence&() { return characters(); }
    ANDROID_EXPORT CharSequence& operator*() { return characters(); }

    // Replaces the specified range (st¡¦en) of text in this Editable with a copy of the slice start¡¦end from source.
    ANDROID_EXPORT virtual Editable& replace(int32_t st, int32_t en, CharSequence source, int32_t start, int32_t end) = 0;
    // Convenience for replace(st, en, text, 0, text.length()) 
    ANDROID_EXPORT virtual Editable& replace(int32_t st, int32_t en, CharSequence text) = 0;

protected:
    ANDROID_EXPORT virtual CharSequence& characters() = 0;
};

} // namespace text
} // namespace android

using Editable = android::text::Editable;
