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

#include <android/content/Context.h>
#include <android/os/Parcelable.h>

namespace android {
namespace content {

class ComponentName final : public Parcelable {
    friend class ComponentNameCreator;
public:
    ANDROID_EXPORT ComponentName() = default;
    // Create a new component identifier from a Context and Class object. 
    ANDROID_EXPORT ComponentName(Context& pkg, Class& cls);
    // Create a new component identifier.
    ANDROID_EXPORT ComponentName(StringRef pkg, StringRef cls);
    // Create a new component identifier from a Context and class name. 
    ANDROID_EXPORT ComponentName(Context& pkg, StringRef cls);
    ANDROID_EXPORT ComponentName(const ComponentName&);
    ANDROID_EXPORT ComponentName(ComponentName&&);
    ANDROID_EXPORT ComponentName& operator=(const ComponentName&);
    ANDROID_EXPORT ComponentName& operator=(ComponentName&&);
    ANDROID_EXPORT ~ComponentName();

    // Return the package name of this component. 
    ANDROID_EXPORT String getPackageName();
    // Return the class name of this component.
    ANDROID_EXPORT String getClassName();
    // Return the class name, either fully qualified or in a shortened form (with a leading '.') if it is a suffix of the package. 
    ANDROID_EXPORT String getShortClassName();

    // Return a String that unambiguously describes both the package and class names contained in the ComponentName. 
    ANDROID_EXPORT String flattenToString();

    // Parcelable
    ANDROID_EXPORT static const std::lazy_ptr<Parcelable::Creator> CREATOR;

    ANDROID_EXPORT int32_t describeContents() override;
    ANDROID_EXPORT void writeToParcel(Parcel& dest, int32_t flags) override;

private:
    String m_packageName;
    String m_className;
};

} // namespace content
} // namespace android

using ComponentName = android::content::ComponentName;
