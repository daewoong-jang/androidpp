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

#include "ApplicationLauncher.h"

#include <Windows.h>
#include <VersionHelpers.h>
#include <shlwapi.h>

#include <array>

#include <android++/StringConversion.h>

namespace android {
namespace content {

static const char* processJobName()
{
    static char name[MAX_PATH];
    _snprintf(name, MAX_PATH, "android++.%ld", ::GetCurrentProcessId());
    return name;
}

static const wchar_t* processExecutableName()
{
    static const wchar_t name[] = L"android++.stub.exe";
    return name;
}

class JobObject {
public:
    ~JobObject();

    static HANDLE handle()
    {
        static JobObject jobObject;
        return jobObject.m_jobObject;
    }

private:
    JobObject();

    HANDLE m_jobObject;
};

JobObject::JobObject()
    : m_jobObject(::CreateJobObjectA(NULL, processJobName()))
{
    // Causes all processes associated with the job to terminate when the
    // last handle to the job is closed.        
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };
    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!IsWindows8OrGreater()) {
        // Windows 8 implements nested jobs, but for older systems we need to
        // break out of any job we're in to enforce our restrictions.
        jeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_BREAKAWAY_OK;
    }

    ::SetInformationJobObject(m_jobObject, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli));
}

JobObject::~JobObject()
{
    ::CloseHandle(m_jobObject);
}

static inline String toCommandLine(std::array<String, 3>&& options)
{
    String result;
    for (auto& option : options) {
        result.append(L"\"");
        result.append(option);
        result.append(L"\" ");
    }

    return result;
}

intptr_t ApplicationLauncher::platformCreateProcess(StringRef component)
{
    STARTUPINFOW startupInfo = { 0 };
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInformation = { 0 };

    // Start the target process suspended.
    DWORD flags = CREATE_SUSPENDED;
    if (!IsWindows8OrGreater()) {
        // Windows 8 implements nested jobs, but for older systems we need to
        // break out of any job we're in to enforce our restrictions.
        flags |= CREATE_BREAKAWAY_FROM_JOB;
    }

    String commandLine = toCommandLine({
        System::getSystemPath() + L"\\" + processExecutableName(),
        std::to_wstring(m_process.handle()),
        component
    });

    BOOL result = ::CreateProcessW(0, const_cast<wchar_t*>(commandLine.c_str()),
        0, 0, true, flags,
        0, 0, &startupInfo, &processInformation);

    if (!result) {
        // FIXME: What should we do here?
        DWORD error = ::GetLastError();
        assert(false);
        return 0;
    }

    if (!IsWindows8OrGreater()) {
        BOOL inJob = FALSE;
        IsProcessInJob(processInformation.hProcess, NULL, &inJob);
        if (inJob) {
            assert(false);
            return 0;
        }
    }

    ::AssignProcessToJobObject(JobObject::handle(), processInformation.hProcess);
    ::ResumeThread(processInformation.hThread);

    // Don't leak the thread handle.
    ::CloseHandle(processInformation.hThread);

    return reinterpret_cast<intptr_t>(processInformation.hProcess);
}

} // namespace content
} // namespace android
