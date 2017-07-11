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
#include <android/os/Handler.h>
#include <android/os/Parcel.h>

namespace android {
namespace app {

class ApplicationProcess final : public Binder::Client {
public:
    class MessageClient {
    public:
        virtual void onTimer() = 0;
        virtual bool onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags) = 0;
    };

    static ApplicationProcess& current();
    ~ApplicationProcess();

    bool isRoot();
    std::passed_ptr<Binder> self();
    std::passed_ptr<Binder> root();

    bool initialize(intptr_t, std::unordered_map<String, String>& parameters);
    bool load(StringRef moduleName);

    intptr_t handle();
    bool setTimeout();
    bool setTimeout(std::chrono::milliseconds);

    bool post(std::function<void ()>&& r);
    bool postDelayed(std::function<void ()>&& r, std::chrono::milliseconds delayMillis);

    void appendMessageClient(MessageClient*);
    void removeMessageClient(MessageClient*);

private:
    ApplicationProcess();

    void platformCreate();
    void platformDestroy();
    bool platformInitialize(std::unordered_map<String, String>& parameters);

    // Binder::Client
    void onCreate() override;
    void onDestroy() override;
    void onTimer() override;
    void onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;

    void* m_module { nullptr };
    std::shared_ptr<Binder> m_self;
    std::shared_ptr<Binder> m_root;
    std::shared_ptr<Handler> m_mainThreadHandler;
    std::vector<MessageClient*> m_messageClients;
};

} // namespace app
} // namespace android

using ApplicationProcess = android::app::ApplicationProcess;
