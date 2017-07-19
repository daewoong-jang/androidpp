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

#include <android/app/MessageClientBase.h>
#include <android/os/Parcel.h>

#include <unordered_map>

namespace android {
namespace content {

class ContextWrapper;
class Intent;
class ServiceConnection;

class ApplicationLauncher final : public MessageClientBase<ApplicationLauncher> {
    class Application;
    friend class Application;
    friend class ContextWrapper;
public:
    ~ApplicationLauncher();

    static const int32_t NO_ACTION = E(0);
    static const int32_t ACK_APPLICATION_LOADER = E(1);
    static const int32_t APPLICATION_INITIALIZED = E(2);
    static const int32_t APPLICATION_STARTED = E(3);
    static const int32_t SERVICE_ON_BIND = E(4);

    static ApplicationLauncher& get();

    std::shared_ptr<Binder> remote(int64_t pid);

    bool bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags);
    void unbindService(std::passed_ptr<ServiceConnection> conn);

private:
    ApplicationLauncher();

    intptr_t platformCreateProcess(StringRef component);

    using ResponseHeaderData = std::pair<Application*, std::pair<intptr_t, int64_t>>;
    ResponseHeaderData readResponseHeader(Parcel& parcel);

    // ApplicationProcess::MessageClient
    void onTimer() override;
    bool onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags) override;

    ApplicationProcess& m_process;
    std::unordered_map<String, std::unique_ptr<Application>> m_applications;
};

} // namespace content
} // namespace android

using ApplicationLauncher = android::content::ApplicationLauncher;
