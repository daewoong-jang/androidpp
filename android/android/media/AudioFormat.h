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
namespace media {

class AudioFormat {
public:
    // The audio stream for music playback.
    static const int32_t STREAM_MUSIC = 3; // FIXME: AudioManager.STREAM_MUSIC

    static const int32_t CHANNEL_OUT_MONO = 4;
    static const int32_t CHANNEL_OUT_STEREO = 12;

    // Audio data format: PCM 16 bit per sample. Guaranteed to be supported by devices.
    static const int32_t ENCODING_PCM_16BIT = 2;
    // Audio data format: PCM 8 bit per sample. Not guaranteed to be supported by devices.
    static const int32_t ENCODING_PCM_8BIT = 3;
    // Audio data format: single-precision floating-point per sample. Added in API level 21.
    static const int32_t ENCODING_PCM_FLOAT = 4;
};

} // namespace media
} // namespace android

using AudioFormat = android::media::AudioFormat;
