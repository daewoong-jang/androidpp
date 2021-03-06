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

#include <android/os/Parcel.h>
#include <java/io/ByteReader.h>
#include <java/io/ByteWriter.h>
#include <unordered_set>
#include <vector>

namespace android {
namespace os {

class Binder;
class ServiceObject;

class ParcelPrivate : public java::io::ByteReader, public java::io::ByteWriter
    , public java::io::ByteBufferProvider {
public:
    ParcelPrivate(Parcel&);
    ~ParcelPrivate();
    static void initializeWithCopy(Parcel&, int8_t* data, size_t length);

    static ParcelPrivate& getPrivate(Parcel&);
    static void setPrivate(Parcel&, std::unique_ptr<ParcelPrivate>&&);

    size_t size() const override;
    int8_t* data() override;

    void reset();
    void setSent();

    void setOrigin(std::passed_ptr<Binder> binder);
    std::shared_ptr<Binder> getOrigin();

    void hold(intptr_t);
    void hold(ServiceObject*);

private:
    void resize(size_t) override;

    Parcel& m_parcel;
    bool m_sent { false };
    std::vector<int8_t> m_buffer;
    std::shared_ptr<Binder> m_origin;
    std::unordered_set<intptr_t> m_handles;
    std::unordered_set<ServiceObject*> m_serviceObjects;
};

} // namespace os
} // namespace android
