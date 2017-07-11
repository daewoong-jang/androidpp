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

#include "ComponentName.h"

#include <android/os/ParcelablePrivate.h>
#include <algorithm>

namespace android {
namespace content {

ComponentName::ComponentName(Context& pkg, Class& cls)
    : m_packageName(pkg.getPackageName())
    , m_className(cls.getName())
{
}

ComponentName::ComponentName(StringRef pkg, StringRef cls)
    : m_packageName(pkg)
    , m_className(cls)
{
}

ComponentName::ComponentName(Context& pkg, StringRef cls)
    : m_packageName(pkg.getPackageName())
    , m_className(cls)
{
}

ComponentName::ComponentName(const ComponentName& other)
    : m_packageName(other.m_packageName)
    , m_className(other.m_className)
{
}

ComponentName::ComponentName(ComponentName&& other)
    : m_packageName(std::move(other.m_packageName))
    , m_className(std::move(other.m_className))
{
}

ComponentName& ComponentName::operator=(const ComponentName& other)
{
    m_packageName = other.m_packageName;
    m_className = other.m_className;
    return *this;
}

ComponentName& ComponentName::operator=(ComponentName&& other)
{
    m_packageName = std::move(other.m_packageName);
    m_className = std::move(other.m_className);
    return *this;
}

ComponentName::~ComponentName()
{
}

String ComponentName::getPackageName()
{
    return m_packageName;
}

String ComponentName::getClassName()
{
    return m_className;
}

String ComponentName::getShortClassName()
{
    if (m_className.find_first_of(m_packageName) == 0)
        return m_className.substr(m_packageName.length());

    return m_className;
}

String ComponentName::flattenToString()
{
    return m_packageName + L'/' + m_className;
}

class ComponentNameCreator final : public android::os::ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        auto result = std::make_shared<ComponentName>();
        source >> result->m_packageName;
        source >> result->m_className;
        return result;
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    ComponentNameCreator()
        : ParcelableCreator(this, L"android.content", L"ComponentName.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> ComponentName::CREATOR([] { return new ComponentNameCreator; });

int32_t ComponentName::describeContents()
{
    return 0;
}

void ComponentName::writeToParcel(Parcel& dest, int32_t flags) const
{
    dest << ParcelableCreator::creator<ComponentName>().binaryName;
    dest << m_packageName;
    dest << m_className;
}

} // namespace content
} // namespace android
