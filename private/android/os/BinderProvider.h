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

#include <android/os/Binder.h>

namespace android {
namespace os {

class BinderProvider {
public:
    class Client {
    public:
        virtual void onCreate() = 0;
        virtual void onDestroy() = 0;
        virtual void onTimer() = 0;
        virtual void onTransaction(int32_t code, Parcel& data, intptr_t replyTo, int32_t flags) = 0;
    };

    static std::unique_ptr<BinderProvider> create(Client&);
    virtual ~BinderProvider() = default;

    virtual intptr_t handle() const = 0;

    virtual bool start() = 0;
    virtual bool startAtTime(std::chrono::milliseconds) = 0;
    virtual void stop() = 0;

    virtual void close() = 0;

    virtual bool transact(Binder* destination, int32_t code, Parcel& data, int32_t flags) = 0;

protected:
    BinderProvider(Client&);

    Client& m_client;
};

} // namespace os
} // namespace android
