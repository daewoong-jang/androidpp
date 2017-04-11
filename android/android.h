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

#include <android/app/Activity.h>
#include <android/app/Service.h>
#include <android/content/ServiceConnection.h>

namespace android {

class ApplicationContext : public Context {
    class Process;
public:
    ANDROID_EXPORT ApplicationContext();
    ANDROID_EXPORT ApplicationContext(String moduleName, String packageName);
    ANDROID_EXPORT ApplicationContext(String moduleName, String packageName, int32_t argc, char** argv);
    ANDROID_EXPORT ApplicationContext(String moduleName, String packageName, int32_t argc, wchar_t** argv);
    ANDROID_EXPORT virtual ~ApplicationContext();

    ANDROID_EXPORT virtual void setApplication(std::passed_ptr<Context> application);
    ANDROID_EXPORT virtual void setApplication(Intent& intent, int32_t action, int32_t flags);

    ANDROID_EXPORT virtual StringRef getModuleName();
    ANDROID_EXPORT virtual Intent& getIntent();
    ANDROID_EXPORT virtual int32_t getAction();
    ANDROID_EXPORT virtual int32_t getFlags();
    ANDROID_EXPORT virtual std::passed_ptr<Bundle> getInstanceStateBundle();
    ANDROID_EXPORT virtual void setInstanceStateBundle(std::passed_ptr<Bundle> stateBundle);
    ANDROID_EXPORT std::shared_ptr<IBinder> getWindowToken();

    ANDROID_EXPORT virtual int32_t runApplication();

    // Context
    ANDROID_EXPORT virtual Context& getApplicationContext() override;
    ANDROID_EXPORT virtual StringRef getPackageName() override;
    ANDROID_EXPORT virtual std::shared_ptr<Object> getSystemService(StringRef name) override;
    ANDROID_EXPORT virtual Resources& getResources() override;

    ANDROID_EXPORT virtual bool bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags) override;
    ANDROID_EXPORT virtual void unbindService(std::passed_ptr<ServiceConnection> conn) override;

private:
    std::unique_ptr<Process> m_process;
    String m_moduleName;
    String m_packageName;
    Intent m_intent;
    int32_t m_action { 0 };
    int32_t m_flags { 0 };
    std::shared_ptr<Context> m_application;
    std::shared_ptr<Bundle> m_stateBundle;
};

} // namespace android

using ApplicationContext = android::ApplicationContext;
