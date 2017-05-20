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

#include "MessengerService.h"

#include <android/os/Handler.h>
#include <android/os/Message.h>
#include <android++/LogHelper.h>

// Bound Services: Using a Messenger example
// https://developer.android.com/guide/components/bound-services.html#Messenger

namespace com {
namespace example {

/**
* Handler of incoming messages from clients.
*/
class IncomingHandler : public Handler {
public:
    void handleMessage(Message& msg) override
    {
        switch (msg.what) {
        case MessengerService::MSG_SAY_HELLO:
            // FIXME: test.
            MessageBoxA(0, "Say Hello!", "Toast", 0);
            break;
        default:
            Handler::handleMessage(msg);
        }
    }
};

MessengerService::MessengerService()
    : mMessenger(std::make_shared<Messenger>(std::make_shared<IncomingHandler>()))
{
}

MessengerService::~MessengerService()
{
}

std::shared_ptr<IBinder> MessengerService::onBind(Intent& intent)
{
    LOGD("%s", __FUNCTION__);
    return mMessenger->getBinder();
}

void MessengerService::onConfigurationChanged(Configuration& newConfig)
{
    LOGD("%s", __FUNCTION__);
}

void MessengerService::onCreate()
{
    LOGD("%s", __FUNCTION__);
}

void MessengerService::onDestroy()
{
    LOGD("%s", __FUNCTION__);
}

void MessengerService::onLowMemory()
{
    LOGD("%s", __FUNCTION__);
}

void MessengerService::onRebind(Intent& intent)
{
    LOGD("%s", __FUNCTION__);
}

int32_t MessengerService::onStartCommand(Intent& intent, int32_t flags, int32_t startId)
{
    LOGD("%s", __FUNCTION__);
    return Service::onStartCommand(intent, flags, startId);
}

void MessengerService::onTaskRemoved(Intent& rootIntent)
{
    LOGD("%s", __FUNCTION__);
}

void MessengerService::onTrimMemory(int32_t level)
{
    LOGD("%s", __FUNCTION__);
}

bool MessengerService::onUnbind(Intent& intent)
{
    LOGD("%s", __FUNCTION__);
    return true;
}

} // namespace example
} // namespace com
