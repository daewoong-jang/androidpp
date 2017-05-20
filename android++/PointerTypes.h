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

#include <functional>
#include <memory>

namespace std {

template<typename T> using passed_ptr = const shared_ptr<T>&;

template<typename T> shared_ptr<T> ref_ptr(T* ptr) { return shared_ptr<T>(ptr, [] (T*) {}); }
template<typename T> shared_ptr<T> ref_ptr(T& ptr) { return shared_ptr<T>(&ptr, [] (T*) {}); }

template<typename T>
class lazy_ptr {
public:
    template<typename F>
    lazy_ptr(F&& ctor)
        : m_ctor(std::move(ctor))
    {
    }
    lazy_ptr(const lazy_ptr&) = delete;
    lazy_ptr& operator=(const lazy_ptr&) = delete;

    operator bool() const
    {
        return !!m_ptr;
    }

    template<typename U>
    bool operator==(const U& other)
    {
        return m_ptr.get() == other.get();
    }
    template<typename U>
    bool operator!=(const U& other)
    {
        return m_ptr.get() != other.get();
    }

    T* operator->() const { return get(); }
    T& operator*() const { return *get(); }

    const std::shared_ptr<T>& ptr() { return m_ptr; }

    T* get() const
    {
        if (!m_init) {
            m_ptr.reset(m_ctor());
            m_init = true;
        }

        return m_ptr.get();
    }

private:
    mutable std::shared_ptr<T> m_ptr;
    mutable bool m_init = false;
    std::function<T* ()> m_ctor;
};

}
