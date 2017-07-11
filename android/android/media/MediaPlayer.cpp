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

#include "MediaPlayer.h"

#include <android/media/MediaPlayerPrivate.h>
#include <android++/LogHelper.h>

#include <algorithm>

namespace android {
namespace media {

MediaPlayer::MediaPlayer()
    : m_private(MediaPlayerPrivate::create(*this))
{
}

MediaPlayer::MediaPlayer(StringRef url)
    : m_private(MediaPlayerPrivate::create(*this))
{
}

MediaPlayer::MediaPlayer(const MediaPlayer& other)
    : m_private(other.m_private)
{
}

MediaPlayer::MediaPlayer(MediaPlayer&& other)
    : m_private(std::move(other.m_private))
{
}

MediaPlayer& MediaPlayer::operator=(const MediaPlayer& other)
{
    m_private = other.m_private;
    return *this;
}

MediaPlayer& MediaPlayer::operator=(MediaPlayer&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

MediaPlayer::~MediaPlayer()
{
}

void MediaPlayer::addTimedTextSource(StringRef path, StringRef mimeType)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::deselectTrack(int32_t index)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

int32_t MediaPlayer::getCurrentPosition()
{
    return m_private->getCurrentPosition();
}

int32_t MediaPlayer::getDuration()
{
    return m_private->getDuration();
}

int32_t MediaPlayer::getSelectedTrack(int32_t trackType)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
    return 0;
}

std::vector<MediaPlayer::TrackInfo> MediaPlayer::getTrackInfo()
{
    return std::vector<MediaPlayer::TrackInfo>();
}

int32_t MediaPlayer::getVideoHeight()
{
    return m_private->getVideoHeight();
}

int32_t MediaPlayer::getVideoWidth()
{
    return m_private->getVideoWidth();
}

bool MediaPlayer::isLooping()
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
    return false;
}

bool MediaPlayer::isPlaying()
{
    return m_private->m_state == MediaPlayerPrivate::Started;
}

void MediaPlayer::pause()
{
    assert(m_private->m_state == MediaPlayerPrivate::Started || m_private->m_state == MediaPlayerPrivate::Paused || m_private->m_state == MediaPlayerPrivate::PlaybackCompleted);

    m_private->stateChanged(MediaPlayerPrivate::Paused);
}

void MediaPlayer::prepare()
{
    assert(m_private->m_state == MediaPlayerPrivate::Initialized || m_private->m_state == MediaPlayerPrivate::Stopped);

    m_private->stateChanged(MediaPlayerPrivate::Prepare);
}

void MediaPlayer::prepareAsync()
{
    assert(m_private->m_state == MediaPlayerPrivate::Initialized || m_private->m_state == MediaPlayerPrivate::Stopped);

    m_private->stateChanged(MediaPlayerPrivate::Preparing);
}

void MediaPlayer::release()
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::reset()
{
    m_private->stateChanged(MediaPlayerPrivate::Idle);
}

void MediaPlayer::seekTo(int32_t msec)
{
    assert(m_private->m_state == MediaPlayerPrivate::Prepared || m_private->m_state == MediaPlayerPrivate::Started || m_private->m_state == MediaPlayerPrivate::Paused || m_private->m_state == MediaPlayerPrivate::PlaybackCompleted);

    msec = std::max(0, std::min(msec, m_private->getDuration()));
    m_private->seekTo(msec);
}

void MediaPlayer::selectTrack(int32_t index)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::setDataSource(StringRef path)
{
    assert(m_private->m_state == MediaPlayerPrivate::Idle);

    m_private->setDataSource(path);
    m_private->stateChanged(MediaPlayerPrivate::Initialized);
}

void MediaPlayer::setDataSource(Context& context, StringRef uri, const std::map<String, String>& headers)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::setLooping(bool looping)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::setOnBufferingUpdateListener(OnBufferingUpdateListener&& listener)
{
    m_private->m_bufferingUpdateListener = std::move(listener);
}

void MediaPlayer::setOnCompletionListener(OnCompletionListener&& listener)
{
    m_private->m_completionListener = std::move(listener);
}

void MediaPlayer::setOnErrorListener(OnErrorListener&& listener)
{
    m_private->m_errorListener = std::move(listener);
}
    
void MediaPlayer::setOnInfoListener(OnInfoListener&& listener)
{
    m_private->m_infoListener = std::move(listener);
}

void MediaPlayer::setOnPreparedListener(OnPreparedListener&& listener)
{
    m_private->m_preparedListener = std::move(listener);
}

void MediaPlayer::setOnSeekCompleteListener(OnSeekCompleteListener&& listener)
{
    m_private->m_seekCompleteListener = std::move(listener);
}

void MediaPlayer::setOnTimedTextListener(OnTimedTextListener&& listener)
{
    m_private->m_timedTextListener = std::move(listener);
}

void MediaPlayer::setOnVideoSizeChangedListener(OnVideoSizeChangedListener&& listener)
{
    m_private->m_videoSizeChangedListener = std::move(listener);
}

void MediaPlayer::setScreenOnWhilePlaying(bool screenOn)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::setVideoScalingMode(int32_t mode)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::setVolume(float leftVolume, float rightVolume)
{
    m_private->setVolume(leftVolume);
}

void MediaPlayer::setWakeMode(int32_t mode)
{
    LOGD("NOT IMPLEMENTED: %s", __FUNCTION__);
}

void MediaPlayer::start()
{
    assert(m_private->m_state == MediaPlayerPrivate::Prepared || m_private->m_state == MediaPlayerPrivate::Started || m_private->m_state == MediaPlayerPrivate::Paused || m_private->m_state == MediaPlayerPrivate::PlaybackCompleted);

    m_private->stateChanged(MediaPlayerPrivate::Started);
}

void MediaPlayer::stop()
{
    assert(m_private->m_state == MediaPlayerPrivate::Prepared || m_private->m_state == MediaPlayerPrivate::Started || m_private->m_state == MediaPlayerPrivate::Stopped || m_private->m_state == MediaPlayerPrivate::Paused || m_private->m_state == MediaPlayerPrivate::PlaybackCompleted);

    m_private->stateChanged(MediaPlayerPrivate::Stopped);
}

void MediaPlayer::setSurface(std::passed_ptr<Surface> surface)
{
    m_private->m_surface = surface;
    m_private->surfaceChanged();
}

} // namespace media
} // namespace android
