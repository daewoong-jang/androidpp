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

#include "PlatformHandle.h"
#include "ServiceObject.h"

namespace android {
namespace os {

class MemoryFilePrivate final : public ServiceObject, public PlatformHandle {
public:
    MemoryFilePrivate(ServiceMessageClient<MemoryFilePrivate>&);
    ~MemoryFilePrivate();

    static void initService();

    bool map(const std::string& name, int32_t length);
    void close();

    void* data();

    void createFromParcel(Parcel&) override;
    void updateFromBundle(Bundle&, int64_t) override;
    void readFromParcel(Parcel&) override;
    void writeToParcel(Parcel&, int32_t flags) const override;

private:
    intptr_t platformMap(const char* name, int32_t length);
    void* platformData();

    std::string m_name;
    int32_t m_length { 0 };
    void* m_data { nullptr };
};

} // namespace os
} // namespace android
