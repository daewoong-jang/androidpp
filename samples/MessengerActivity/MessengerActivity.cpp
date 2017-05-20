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

#include "MessengerActivity.h"

#include "MessengerService.h"
#include <android/os/Message.h>
#include <android/view/InputDevice.h>
#include <android++/LogHelper.h>

// Bound Services: Using a Messenger example
// https://developer.android.com/guide/components/bound-services.html#Messenger

namespace com {
namespace example {

class MessengerServiceConnection : public ServiceConnection {
    friend class MessengerActivity;
public:
    void onServiceConnected(ComponentName& className, std::passed_ptr<IBinder> service) {
        // This is called when the connection with the service has been
        // established, giving us the object we can use to
        // interact with the service.  We are communicating with the
        // service using a Messenger, so here we get a client-side
        // representation of that from the raw IBinder object.
        mThis.mService = std::make_shared<Messenger>(service);
        mThis.mBound = true;
    }

    void onServiceDisconnected(ComponentName& className) {
        // This is called when the connection with the service has been
        // unexpectedly disconnected -- that is, its process crashed.
        mThis.mService = nullptr;
        mThis.mBound = false;
    }

private:
    MessengerServiceConnection(MessengerActivity& activity)
        : mThis(activity)
    {
    }

    MessengerActivity& mThis;
};

MessengerActivity::MessengerActivity()
    : mConnection(new MessengerServiceConnection(*this))
{
}

MessengerActivity::~MessengerActivity()
{
}

void MessengerActivity::sayHello()
{
    if (!mBound)
        return;
    // Create and send a message to the service, using a supported 'what' value
    Message msg = Message::obtain(nullptr, MessengerService::MSG_SAY_HELLO, 0, 0);
    mService->send(msg);
}

bool MessengerActivity::onGenericMotionEvent(MotionEvent& event)
{
    LOGD("%s", __FUNCTION__);
    if (event.getSource() == InputDevice::SOURCE_MOUSE &&
        event.getAction() == MotionEvent::ACTION_BUTTON_RELEASE)
        sayHello();
    return false;
}

void MessengerActivity::onCreate(std::passed_ptr<Bundle> savedInstanceState)
{
    LOGD("%s", __FUNCTION__);
    Activity::onCreate(savedInstanceState);
}

void MessengerActivity::onStart()
{
    LOGD("%s", __FUNCTION__);
    Activity::onStart();
    // Bind to the service
    bindService(Intent(*this, classT<MessengerService>()), mConnection,
        Context::BIND_AUTO_CREATE);
}

void MessengerActivity::onStop()
{
    LOGD("%s", __FUNCTION__);
    Activity::onStop();
    // Unbind from the service
    if (mBound) {
        unbindService(mConnection);
        mBound = false;
    }
}

} // namespace example
} // namespace com
