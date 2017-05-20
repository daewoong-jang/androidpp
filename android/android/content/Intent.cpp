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

#include "Intent.h"

#include <android/content/IntentPrivate.h>
#include <android/os/BundlePrivate.h>
#include <android/os/ParcelablePrivate.h>
#include <android++/LogHelper.h>

namespace android {
namespace content {

Intent::Intent()
    : m_private(std::make_shared<IntentPrivate>())
{
}

Intent::Intent(Context& packageContext, Class& cls)
    : m_private(std::make_shared<IntentPrivate>())
{
    setClass(packageContext, cls);
}

Intent::Intent(const Intent& other)
    : m_private(other.m_private)
{
}

Intent::Intent(Intent&& other)
    : m_private(std::move(other.m_private))
{
}

Intent& Intent::operator=(const Intent& other)
{
    m_private = other.m_private;
    return *this;
}

Intent& Intent::operator=(Intent&& other)
{
    m_private = std::move(other.m_private);
    return *this;
}

Intent::~Intent()
{
}

ComponentName& Intent::getComponent()
{
    return m_private->m_componentName;
}

Intent& Intent::setComponent(ComponentName& component)
{
    m_private->m_componentName = component;
    return *this;
}

StringRef Intent::getPackage()
{
    return m_private->m_componentName.getPackageName();
}

Intent& Intent::setClass(Context& packageContext, Class& cls)
{
    m_private->m_componentName = ComponentName(packageContext, cls);
    return *this;
}

Intent& Intent::setClassName(Context& packageContext, StringRef className)
{
    m_private->m_componentName = ComponentName(packageContext, className);
    return *this;
}

Intent& Intent::setClassName(StringRef packageName, StringRef className)
{
    m_private->m_componentName = ComponentName(packageName, className);
    return *this;
}

Bundle& Intent::getExtras()
{
    return m_private->m_extras;
}

bool Intent::hasExtra(StringRef name)
{
    return m_private->m_extras.containsKey(name);
}

int32_t Intent::getIntExtra(StringRef name, int32_t defaultValue)
{
    os::BundlePrivate& extras = os::BundlePrivate::getPrivate(m_private->m_extras);
    if (!extras.findKey(name))
        return defaultValue;

    return extras.getValue(name).i;
}

Intent& Intent::putExtra(StringRef name, int32_t value)
{
    os::BundlePrivate::getPrivate(m_private->m_extras).putValue(name, os::BundleValue(value));
    return *this;
}

CharSequence Intent::getCharSequenceExtra(StringRef name)
{
    return m_private->m_extras.getCharSequence(name);
}

Intent& Intent::putExtra(StringRef name, const CharSequence& value)
{
    m_private->m_extras.putCharSequence(name, value);
    return *this;
}

std::shared_ptr<Parcelable> Intent::getParcelableExtra(StringRef name)
{
    return m_private->m_extras.getParcelable(name);
}

Intent& Intent::putExtra(StringRef name, std::passed_ptr<Parcelable> value)
{
    if (value->describeContents() == Parcelable::CONTENTS_FILE_DESCRIPTOR) {
        LOGA("RuntimeException: Not allowed to write file descriptor here");
        return *this;
    }

    m_private->m_extras.putParcelable(name, value);
    return *this;
}

class IntentCreator final : public android::os::ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        auto result = std::make_shared<Intent>();
        auto componentName = std::static_pointer_cast<ComponentName>(android::os::ParcelablePrivate::createFromParcel(source));
        auto extras = std::static_pointer_cast<Bundle>(android::os::ParcelablePrivate::createFromParcel(source));
        auto privateExtras = std::static_pointer_cast<Bundle>(android::os::ParcelablePrivate::createFromParcel(source));
        String moduleName;
        source >> moduleName;
        result->m_private->m_componentName = std::move(*componentName);
        result->m_private->m_extras = std::move(*extras);
        result->m_private->m_privateExtras = std::move(*extras);
        result->m_private->setModuleName(moduleName);
        return std::move(result);
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    IntentCreator()
        : ParcelableCreator(this, L"android.content", L"Intent.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> Intent::CREATOR([] { return new IntentCreator; });

int32_t Intent::describeContents()
{
    return 0;
}

void Intent::writeToParcel(Parcel& dest, int32_t flags)
{
    dest << ParcelableCreator::creator<Intent>().binaryName;
    m_private->m_componentName.writeToParcel(dest, flags);
    m_private->m_extras.writeToParcel(dest, flags);
    m_private->m_privateExtras.writeToParcel(dest, flags);
    dest << m_private->getModuleName();
}

} // namespace content
} // namespace android
