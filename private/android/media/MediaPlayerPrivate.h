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

#include <android/media/MediaPlayer.h>

namespace android {
namespace media {

class MediaPlayerPrivate : public Object {
    friend class MediaPlayer;
public:  
    static std::shared_ptr<MediaPlayerPrivate> create(MediaPlayer&);
    virtual ~MediaPlayerPrivate() = default;

    static MediaPlayerPrivate& getPrivate(MediaPlayer&);

    enum State {
        Unknown,
        Idle,
        Initialized,
        Prepare,
        Preparing,
        Prepared,
        Started,
        Paused,
        Stopped,
        PlaybackCompleted,
        End,
        Error,
    };

    virtual void setDataSource(StringRef path) = 0;
    virtual void setVolume(float) = 0;

    virtual int32_t getDuration() = 0;
    virtual int32_t getCurrentPosition() = 0;
    virtual void seekTo(int32_t msec) = 0;

    virtual int32_t getVideoHeight() = 0;
    virtual int32_t getVideoWidth() = 0;

    // Called by MediaPlayer whenever its state is being changed.
    virtual void stateChanged(State oldState, State newState) = 0;

    virtual void setOnFrameAvailableListener(std::function<void ()>&& l) { m_frameAvailableListener = std::move(l); }

protected:
    MediaPlayerPrivate(MediaPlayer& mp)
        : m_player(mp)
    {
    }

    void stateChanged(int32_t);
    void surfaceChanged();

    void notifyOnBufferingUpdate(int32_t percent);
    void notifyOnCompletion();
    void notifyOnError(int32_t error);
    void notifyOnInfo();
    void notifyOnPrepared();
    void notifyOnSeekComplete();
    void notifyOnTimedText();
    void notifyOnVideoSizeChanged(int32_t width, int32_t height);

    MediaPlayer& m_player;
    int32_t m_state { Idle };
    int32_t m_videoWidth { 0 };
    int32_t m_videoHeight { 0 };

    MediaPlayer::OnBufferingUpdateListener m_bufferingUpdateListener;
    MediaPlayer::OnCompletionListener m_completionListener;
    MediaPlayer::OnErrorListener m_errorListener;
    MediaPlayer::OnInfoListener m_infoListener;
    MediaPlayer::OnPreparedListener m_preparedListener;
    MediaPlayer::OnSeekCompleteListener m_seekCompleteListener;
    MediaPlayer::OnTimedTextListener m_timedTextListener;
    MediaPlayer::OnVideoSizeChangedListener m_videoSizeChangedListener;

    std::shared_ptr<Surface> m_surface;
    std::function<void ()> m_frameAvailableListener;
};

} // namespace media
} // namespace android
