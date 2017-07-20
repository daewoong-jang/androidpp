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

#include <android.h>
#include <android/app/Activity.h>
#include <android/os/Messenger.h>
#include <android/os/Parcelable.h>

namespace com {
namespace example {

class MediaView;
class MediaSurface;

class MediaPlayerServiceActivity : public Activity {
    friend class MediaPlayerServiceConnection;
public:
    ANDROID_EXTERN MediaPlayerServiceActivity();
    ANDROID_EXTERN ~MediaPlayerServiceActivity();

    void sayHello();

    bool onGenericMotionEvent(MotionEvent& event) override;

protected:
    void onCreate(std::passed_ptr<Bundle> savedInstanceState) override;
    void onStart() override;
    void onStop() override;

private:
    std::shared_ptr<MediaView> mView;
    std::shared_ptr<MediaSurface> mSurface;
    std::shared_ptr<Parcelable> mObject;
    std::shared_ptr<Messenger> mService;
    bool mBound { false };
    std::shared_ptr<ServiceConnection> mConnection;
};

} // namespace example
} // namespace com
