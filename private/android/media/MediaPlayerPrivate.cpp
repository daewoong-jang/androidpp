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

#include "MediaPlayerPrivate.h"

#include <android/media/mock/MediaPlayerPrivateMock.h>
#include <android/view/SurfacePrivate.h>

namespace android {
namespace media {

static MediaPlayerPrivate::State toState(int32_t state)
{
    switch (state) {
    case MediaPlayerPrivate::Unknown:
    case MediaPlayerPrivate::Idle:
    case MediaPlayerPrivate::Initialized:
    case MediaPlayerPrivate::Prepare:
    case MediaPlayerPrivate::Preparing:
    case MediaPlayerPrivate::Prepared:
    case MediaPlayerPrivate::Started:
    case MediaPlayerPrivate::Paused:
    case MediaPlayerPrivate::Stopped:
    case MediaPlayerPrivate::PlaybackCompleted:
    case MediaPlayerPrivate::End:
    case MediaPlayerPrivate::Error:
        return static_cast<MediaPlayerPrivate::State>(state);
    default:
        assert(false);
        break;
    };
    return MediaPlayerPrivate::Error;
}

std::shared_ptr<MediaPlayerPrivate> MediaPlayerPrivate::create(MediaPlayer& mp)
{
    return std::make_shared<MediaPlayerPrivateMock>(mp);
}

MediaPlayerPrivate& MediaPlayerPrivate::getPrivate(MediaPlayer& mp)
{
    return *mp.m_private;
}

void MediaPlayerPrivate::stateChanged(int32_t newState)
{
    MediaPlayerPrivate::State oldState = toState(m_state);
    m_state = newState;

    stateChanged(oldState, toState(newState));
}

void MediaPlayerPrivate::surfaceChanged()
{
    if (!m_surface)
        return;

    if (m_videoWidth == 0 || m_videoHeight == 0)
        return;

    view::SurfacePrivate& surface = view::SurfacePrivate::getPrivate(*m_surface);
    surface.resize(*m_surface, m_videoWidth, m_videoHeight);
}

void MediaPlayerPrivate::notifyOnBufferingUpdate(int32_t percent)
{
    if (m_bufferingUpdateListener)
        m_bufferingUpdateListener(percent);
}

void MediaPlayerPrivate::notifyOnCompletion()
{
    stateChanged(PlaybackCompleted);

    if (m_completionListener)
        m_completionListener();
}

void MediaPlayerPrivate::notifyOnError(int32_t error)
{
    stateChanged(Error);

    if (m_errorListener)
        m_errorListener(error);
}

void MediaPlayerPrivate::notifyOnInfo()
{
    if (m_infoListener)
        m_infoListener();
}

void MediaPlayerPrivate::notifyOnPrepared()
{
    int32_t oldState = m_state;
    stateChanged(Prepared);
    if (oldState == Preparing && m_preparedListener)
        m_preparedListener();
}

void MediaPlayerPrivate::notifyOnSeekComplete()
{
    if (m_seekCompleteListener)
        m_seekCompleteListener();
}

void MediaPlayerPrivate::notifyOnTimedText()
{
    if (m_timedTextListener)
        m_timedTextListener();
}

void MediaPlayerPrivate::notifyOnVideoSizeChanged(int32_t width, int32_t height)
{
    if (width == m_videoWidth && height == m_videoHeight)
        return;

    m_videoWidth = width;
    m_videoHeight = height;

    surfaceChanged();

    if (m_videoSizeChangedListener)
        m_videoSizeChangedListener(width, height);
}

} // namespace media
} // namespace android
