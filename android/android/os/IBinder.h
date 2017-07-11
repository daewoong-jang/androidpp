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

#include <java/lang.h>

namespace android {
namespace os {

class Parcel;

class IBinder {
    NONCOPYABLE(IBinder);
public:
    virtual ~IBinder() = default;

    // The first transaction code available for user commands.
    static const int32_t FIRST_CALL_TRANSACTION = 0x00000001;
    // The last transaction code available for user commands. 
    static const int32_t LAST_CALL_TRANSACTION = 0x00ffffff;
    
    // IBinder protocol transaction code: interrogate the recipient side of the transaction for its canonical interface descriptor. 
    static const int32_t INTERFACE_TRANSACTION = 0x5f4e5446;
    // IBinder protocol transaction code: pingBinder(). 
    static const int32_t PING_TRANSACTION = 0x5f504e47;
    // IBinder protocol transaction code: dump internal state.
    static const int32_t DUMP_TRANSACTION = 0x5f444d50;
    // IBinder protocol transaction code: tell an app asynchronously that the caller likes it. 
    static const int32_t LIKE_TRANSACTION = 0x5f4c494b;
    // IBinder protocol transaction code: send a tweet to the target object. 
    static const int32_t TWEET_TRANSACTION = 0x5f545754;

    // Flag to transact(int, Parcel, Parcel, int): this is a one-way call, meaning that the caller returns immediately, without waiting for a result from the callee. 
    static const int32_t FLAG_ONEWAY = 0x00000001;

    // Perform a generic operation with the object. 
    virtual bool transact(int32_t code, Parcel& data, Parcel* reply, int32_t flags) = 0;

protected:
    IBinder() = default;
};

} // namespace os
} // namespace android

using IBinder = android::os::IBinder;
