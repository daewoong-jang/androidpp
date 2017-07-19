/*
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "Noncopyable.h"
#include <type_traits>
#include <utility>

namespace WTF {

template<typename T> class NeverDestroyed {
    NONCOPYABLE(NeverDestroyed);
public:
    template<typename... Args>
    NeverDestroyed(Args&&... args)
    {
        new (asPtr()) T(std::forward<Args>(args)...);
    }

    operator T&() { return *asPtr(); }
    T& get() { return *asPtr(); }

    T* operator->() { return asPtr(); }

private:
    typedef typename std::remove_const<T>::type* PointerType;

    PointerType asPtr() { return reinterpret_cast<PointerType>(&m_storage); }

    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_storage;
};

template<typename T> class LazyNeverDestroyed {
    NONCOPYABLE(LazyNeverDestroyed);
public:
    template<typename... Args>
    LazyNeverDestroyed(Args&&... args)
        : m_constructor([=] (void* ptr) { new (ptr) T(args...); })
    {
    }

    void construct()
    {
#if !defined(NDEBUG)
        assert(!m_isConstructed);

        m_isConstructed = true;
#endif
        m_constructor(asPtr());
    }

    operator T&() { return *asPtr(); }
    T& get() { return *asPtr(); }

    T* operator->() { return asPtr(); }

private:
    typedef typename std::remove_const<T>::type* PointerType;

    PointerType asPtr()
    {
        if (!m_isConstructed)
            construct();

        return reinterpret_cast<PointerType>(&m_storage);
    }

    typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type m_storage;

    std::function<void (void*)> m_constructor;

#if !defined(NDEBUG)
    // LazyNeverDestroyed objects are always static, so this variable is initialized to false.
    // It must not be initialized dynamically, because that would not be thread safe.
    bool m_isConstructed;
#endif
};

} // namespace WTF

using WTF::LazyNeverDestroyed;
using WTF::NeverDestroyed;
