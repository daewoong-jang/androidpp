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

#include <java/lang.h>

namespace android {
namespace os {

class ServiceObject;

class ServiceObjectRef {
    NONCOPYABLE(ServiceObjectRef);
public:
    ServiceObjectRef(std::passed_ptr<ServiceObject> ptr)
        : m_weakPtr(ptr)
    {
    }
    ~ServiceObjectRef()
    {
        assert(m_weakPtr.expired());
    }

    void ref()
    {
        ptr();
        ++m_refCount;
    }
    void deref()
    {
        if (--m_refCount == 0) {
            m_weakPtr = m_ptr;
            m_ptr.reset();
        }
    }

    std::shared_ptr<ServiceObject> ptr()
    {
        if (!m_ptr) {
            if (m_weakPtr.expired())
                return nullptr;
            m_ptr = m_weakPtr.lock();
        }

        assert(!!m_ptr);
        return m_ptr;
    }

private:
    size_t m_refCount { 0 };
    std::shared_ptr<ServiceObject> m_ptr;
    std::weak_ptr<ServiceObject> m_weakPtr;
};

} // namespace os
} // namespace android
