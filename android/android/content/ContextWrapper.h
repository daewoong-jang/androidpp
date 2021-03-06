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

#include <android/content/Context.h>

namespace android {
namespace content {

class ContextWrapper : public Context {
    NONCOPYABLE(ContextWrapper);
public:
    ANDROID_EXPORT ContextWrapper(Context& base);
    ANDROID_EXPORT virtual ~ContextWrapper();

    ANDROID_EXPORT virtual Context& getApplicationContext() override;
    ANDROID_EXPORT virtual StringRef getPackageName() override;
    ANDROID_EXPORT virtual std::shared_ptr<Object> getSystemService(StringRef name) override;
    ANDROID_EXPORT virtual Resources& getResources() override;

    ANDROID_EXPORT virtual bool bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags) override;
    ANDROID_EXPORT virtual void unbindService(std::passed_ptr<ServiceConnection> conn) override;

private:
    Context& m_base;
};

} // namespace content
} // namespace android

using ContextWrapper = android::content::ContextWrapper;
