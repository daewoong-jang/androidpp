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

#include "IntentPrivate.h"

#include <android/content/ContextPrivate.h>
#include <android++/StringConversion.h>

namespace android {
namespace content {

IntentPrivate::IntentPrivate()
{
}

IntentPrivate& IntentPrivate::getPrivate(Intent& intent)
{
    return *intent.m_private;
}

void IntentPrivate::setPrivate(Intent& intent, std::unique_ptr<IntentPrivate>&& intentPrivate)
{
    intent.m_private = std::move(intentPrivate);
}

StringRef IntentPrivate::getModuleName()
{
    if (m_moduleName.empty())
        m_moduleName = ContextPrivate::getGlobalContext().getModuleName();

    assert(!m_moduleName.empty());
    return m_moduleName;
}

void IntentPrivate::setModuleName(StringRef moduleName)
{
    assert(m_moduleName.empty());
    m_moduleName = moduleName;
}

Bundle& IntentPrivate::getPrivateExtras()
{
    return m_privateExtras;
}

void IntentPrivate::putArgumentExtras(int32_t argc, char** argv)
{
    if (argc == 0 || !argv)
        return;

    for (int32_t i = 0; i < argc; ++i)
        m_extras.putCharSequence(String(L"arg") + std::to_wstring(i), std::s2ws(std::string(argv[i])));
}

void IntentPrivate::putArgumentExtras(int32_t argc, wchar_t** argv)
{
    if (argc == 0 || !argv)
        return;

    for (int32_t i = 0; i < argc; ++i)
        m_extras.putCharSequence(String(L"arg") + std::to_wstring(i), String(argv[i]));
}

} // namespace content
} // namespace android
