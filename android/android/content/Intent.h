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

#include <android/content/ComponentName.h>
#include <android/os/Bundle.h>

namespace android {
namespace content {

class Intent : public Parcelable {
    friend class IntentCreator;
    friend class IntentPrivate;
public:
    ANDROID_EXPORT Intent();
    ANDROID_EXPORT Intent(Context& packageContext, Class& cls);
    ANDROID_EXPORT Intent(const Intent&);
    ANDROID_EXPORT Intent(Intent&&);
    ANDROID_EXPORT Intent& operator=(const Intent&);
    ANDROID_EXPORT Intent& operator=(Intent&&);
    ANDROID_EXPORT virtual ~Intent();

    // Retrieve the concrete component associated with the intent. 
    ANDROID_EXPORT virtual ComponentName& getComponent();
    // Retrieve the application package name this Intent is limited to. 
    ANDROID_EXPORT virtual StringRef getPackage();
    // (Usually optional) Explicitly set the component to handle the intent. 
    ANDROID_EXPORT virtual Intent& setComponent(ComponentName& component);
    // Convenience for calling setComponent(ComponentName) with the name returned by a Class object. 
    ANDROID_EXPORT virtual Intent& setClass(Context& packageContext, Class& cls);
    // Convenience for calling setComponent(ComponentName) with an explicit class name. 
    ANDROID_EXPORT virtual Intent& setClassName(Context& packageContext, StringRef className);
    // Convenience for calling setComponent(ComponentName) with an explicit application package name and class name. 
    ANDROID_EXPORT virtual Intent& setClassName(StringRef packageName, StringRef className);
    // Retrieves a map of extended data from the intent. 
    ANDROID_EXPORT virtual Bundle& getExtras();
    // Returns true if an extra value is associated with the given name. 
    ANDROID_EXPORT virtual bool hasExtra(StringRef name);
    // Retrieve extended data from the intent. 
    ANDROID_EXPORT virtual int32_t getIntExtra(StringRef name, int32_t defaultValue);
    // Add extended data to the intent. 
    ANDROID_EXPORT virtual Intent& putExtra(StringRef name, int32_t value);
    // Retrieve extended data from the intent. 
    ANDROID_EXPORT virtual CharSequence getCharSequenceExtra(StringRef name);
    // Add extended data to the intent. 
    ANDROID_EXPORT virtual Intent& putExtra(StringRef name, const CharSequence& value);
    // Retrieve extended data from the intent. 
    ANDROID_EXPORT virtual std::shared_ptr<Parcelable> getParcelableExtra(StringRef name);
    // Add extended data to the intent. 
    ANDROID_EXPORT virtual Intent& putExtra(StringRef name, std::passed_ptr<Parcelable> value);

    // Parcelable
    ANDROID_EXPORT static const std::lazy_ptr<Parcelable::Creator> CREATOR;

    ANDROID_EXPORT virtual int32_t describeContents() override;
    ANDROID_EXPORT virtual void writeToParcel(Parcel& dest, int32_t flags) override;

private:
    std::shared_ptr<IntentPrivate> m_private;
};

} // namespace content
} // namespace android

using Intent = android::content::Intent;
