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
#include <vector>

namespace android {
namespace os {

class Binder;

class ParcelPrivate {
public:
    ParcelPrivate(Parcel&);
    ParcelPrivate(Parcel&, const int8_t* dataBegin, const int8_t* dataEnd);

    static ParcelPrivate& getPrivate(Parcel&);
    static void setPrivate(Parcel&, std::unique_ptr<ParcelPrivate>&&);
    static void initializeWithCopy(Parcel&, int8_t* data, size_t length);

    int32_t size();
    int8_t* data();

    void reset();

    void read(void* out, size_t length, size_t alignment);
    void write(const void* in, size_t length, size_t alignment);

    int8_t* readArray(size_t& length, size_t alignment);
    void writeArray(const void* in, size_t length, size_t alignment);

    bool hasOrigin();
    void setOrigin(std::passed_ptr<Binder> binder);
    std::shared_ptr<Binder> getOrigin();

private:
    int8_t* grow(size_t length, size_t alignment);
    int8_t* move(size_t length, size_t alignment);

    void throwException();

    Parcel& m_parcel;
    int8_t* m_pointer { nullptr };
    std::vector<int8_t> m_buffer;
    std::shared_ptr<Binder> m_origin;
};

} // namespace os
} // namespace android
