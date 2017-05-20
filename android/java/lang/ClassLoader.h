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

#include <android++/ExportMacros.h>
#include <android++/Noncopyable.h>
#include <android++/PointerTypes.h>
#include <java/lang/StringImport.h>
#include <java/net.h>

#include <unordered_map>

namespace java {
namespace lang {

class Class;
class Package;

class ClassLoader {
    NONCOPYABLE(ClassLoader)
    friend class ClassLoader;
public:
    ANDROID_EXPORT ClassLoader();
    ANDROID_EXPORT virtual ~ClassLoader();

    // Returns the system class loader for delegation.
    ANDROID_EXPORT static ClassLoader& getSystemClassLoader();
    // Defines a package by name in this ClassLoader. 
    ANDROID_EXPORT virtual std::passed_ptr<Package> definePackage(StringRef name, StringRef specTitle, StringRef specVersion, StringRef specVendor, StringRef implTitle, StringRef implVersion, StringRef implVendor, URL& sealBase);
    // Returns a Package that has been defined by this class loader or any of its ancestors. 
    ANDROID_EXPORT virtual std::passed_ptr<Package> getPackage(StringRef name);
    // Finds the class with the specified binary name.
    ANDROID_EXPORT virtual std::passed_ptr<Class> findClass(StringRef name);
    // Links the specified class. 
    ANDROID_EXPORT void resolveClass(std::passed_ptr<Class> c);

private:
    std::unordered_map<String, std::shared_ptr<Package>> m_packages;
    std::unordered_map<String, std::shared_ptr<Class>> m_classes;
};

} // namespace lang
} // namespace java

using ClassLoader = java::lang::ClassLoader;
