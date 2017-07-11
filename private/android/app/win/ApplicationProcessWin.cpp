/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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

#include "ApplicationProcess.h"

#include <shlwapi.h>

namespace android {
namespace app {

static const wchar_t instanceHandleKey[] = L"hInstance";
static const wchar_t prevInstanceHandleKey[] = L"hPrevInstance";
static const wchar_t cmdShowKey[] = L"nCmdShow";

// Code from Webkit (https://webkit.org/) under LGPL v2 and BSD licenses (https://webkit.org/licensing-webkit/)
static void enableDataExecutionPrevention()
{
    // Enable Data Execution prevention at runtime rather than via /NXCOMPAT
    // http://blogs.msdn.com/michael_howard/archive/2008/01/29/new-nx-apis-added-to-windows-vista-sp1-windows-xp-sp3-and-windows-server-2008.aspx

    const DWORD enableDEP = 0x00000001;

    HMODULE hMod = ::GetModuleHandleW(L"Kernel32.dll");
    if (!hMod)
        return;

    typedef BOOL (WINAPI *PSETDEP)(DWORD);

    PSETDEP procSet = reinterpret_cast<PSETDEP>(::GetProcAddress(hMod, "SetProcessDEPPolicy"));
    if (!procSet)
        return;

    // Enable Data Execution Prevention, but allow ATL thunks (for compatibility with the version of ATL that ships with the Platform SDK).
    procSet(enableDEP);
}

static void enableTerminationOnHeapCorruption()
{
    // Enable termination on heap corruption on OSes that support it (Vista and XPSP3).
    // http://msdn.microsoft.com/en-us/library/aa366705(VS.85).aspx

    const HEAP_INFORMATION_CLASS heapEnableTerminationOnCorruption = static_cast<HEAP_INFORMATION_CLASS>(1);

    HMODULE hMod = ::GetModuleHandleW(L"kernel32.dll");
    if (!hMod)
        return;

    typedef BOOL (WINAPI*HSI)(HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);
    HSI heapSetInformation = reinterpret_cast<HSI>(::GetProcAddress(hMod, "HeapSetInformation"));
    if (!heapSetInformation)
        return;

    heapSetInformation(0, heapEnableTerminationOnCorruption, 0, 0);
}

static void disableUserModeCallbackExceptionFilter()
{
    const DWORD PROCESS_CALLBACK_FILTER_ENABLED = 0x1;
    typedef BOOL (NTAPI *getProcessUserModeExceptionPolicyPtr)(LPDWORD lpFlags);
    typedef BOOL (NTAPI *setProcessUserModeExceptionPolicyPtr)(DWORD dwFlags);

    HMODULE lib = LoadLibrary(TEXT("kernel32.dll"));
    assert(lib);

    getProcessUserModeExceptionPolicyPtr getPolicyPtr = (getProcessUserModeExceptionPolicyPtr)GetProcAddress(lib, "GetProcessUserModeExceptionPolicy");
    setProcessUserModeExceptionPolicyPtr setPolicyPtr = (setProcessUserModeExceptionPolicyPtr)GetProcAddress(lib, "SetProcessUserModeExceptionPolicy");

    DWORD dwFlags;
    if (!getPolicyPtr || !setPolicyPtr || !getPolicyPtr(&dwFlags)) {
        FreeLibrary(lib);
        return;
    }

    // If this flag isn't cleared, exceptions that are thrown when running in a 64-bit version of
    // Windows are ignored, possibly leaving Safari in an inconsistent state that could cause an 
    // unrelated exception to be thrown.
    // http://support.microsoft.com/kb/976038
    // http://blog.paulbetts.org/index.php/2010/07/20/the-case-of-the-disappearing-onload-exception-user-mode-callback-exceptions-in-x64/
    setPolicyPtr(dwFlags & ~PROCESS_CALLBACK_FILTER_ENABLED);

    FreeLibrary(lib);
}

#ifndef NDEBUG
static void pauseProcessIfNeeded(HMODULE module)
{
    // Show an alert when Ctrl-Alt-Shift is held down during launch to give the user time to attach a
    // debugger. This is useful for debugging problems that happen early in the process's lifetime.
    const unsigned short highBitMaskShort = 0x8000;
    if (!((::GetKeyState(VK_CONTROL) & highBitMaskShort) && (::GetKeyState(VK_MENU) & highBitMaskShort) && (::GetKeyState(VK_SHIFT) & highBitMaskShort)))
        return;

    wchar_t path[MAX_PATH];
    DWORD length = ::GetModuleFileNameW(module, path, sizeof(path));
    if (!length || length == sizeof(path))
        return;

    wchar_t* startOfFilename = ::PathFindFileNameW(path);
    String filenameString(startOfFilename, length - (startOfFilename - path));

    String message = L"You can now attach a debugger to " + filenameString + L". Click OK when you are ready for " + filenameString + L" to continue.";
    String title = filenameString + L" has launched";
    ::MessageBoxW(0, message.c_str(), title.c_str(), MB_OK | MB_ICONINFORMATION);
}
#endif

void ApplicationProcess::platformCreate()
{
    HRESULT hRes = ::OleInitialize(NULL);
    assert(SUCCEEDED(hRes));
}

void ApplicationProcess::platformDestroy()
{
    ::OleUninitialize();
}

bool ApplicationProcess::platformInitialize(std::unordered_map<String, String>& parameters)
{
    enableDataExecutionPrevention();
    enableTerminationOnHeapCorruption();
    disableUserModeCallbackExceptionFilter();

    return true;
}

bool ApplicationProcess::load(StringRef moduleName)
{
    HMODULE module = ::LoadLibraryW(moduleName.c_str());
    if (!module) {
        assert(false);
        return false;
    }

    m_module = module;
#ifndef NDEBUG
    pauseProcessIfNeeded(module);
#endif
    return true;
}

} // namespace app
} // namespace android
