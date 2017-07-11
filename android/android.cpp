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

#include "android.h"

#include <android/app/ActivityHostWindow.h>
#include <android/app/ActivityPrivate.h>
#include <android/app/ApplicationProcess.h>
#include <android/app/ServiceHostWindow.h>
#include <android/app/ServicePrivate.h>
#include <android/content/ApplicationLauncher.h>
#include <android/content/ContextPrivate.h>
#include <android/content/IntentPrivate.h>
#include <android/os/Looper.h>
#include <android/view/inputmethod/InputMethodManager.h>
#include <android++/LogHelper.h>

namespace android {

class ApplicationContext::Process {
public:
    app::ApplicationProcess& process;

    Process();
};

ApplicationContext::Process::Process()
    : process(app::ApplicationProcess::current())
{
    std::unordered_map<String, String> parameters;
    if (!process.initialize(0, parameters)) {
        LOGA("Process initialization failed");
    }
}

ApplicationContext::ApplicationContext()
    : m_process(std::make_unique<Process>())
{
    content::ContextPrivate::setGlobalContext(*this);
    content::ContextPrivate::getPrivate(*this).setAsApplicationContext();
}

ApplicationContext::ApplicationContext(String moduleName, String packageName)
    : m_process(std::make_unique<Process>())
    , m_moduleName(std::move(moduleName))
    , m_packageName(std::move(packageName))
{
    content::ContextPrivate::setGlobalContext(*this);
    content::ContextPrivate::getPrivate(*this).setAsApplicationContext();
}

ApplicationContext::ApplicationContext(String moduleName, String packageName, int32_t argc, char** argv)
    : m_process(std::make_unique<Process>())
    , m_moduleName(std::move(moduleName))
    , m_packageName(std::move(packageName))
{
    content::ContextPrivate::setGlobalContext(*this);
    content::ContextPrivate::getPrivate(*this).setAsApplicationContext();
    content::IntentPrivate::getPrivate(m_intent).putArgumentExtras(argc, argv);
}

ApplicationContext::ApplicationContext(String moduleName, String packageName, int32_t argc, wchar_t** argv)
    : m_process(std::make_unique<Process>())
    , m_moduleName(std::move(moduleName))
    , m_packageName(std::move(packageName))
{
    content::ContextPrivate::setGlobalContext(*this);
    content::ContextPrivate::getPrivate(*this).setAsApplicationContext();
    content::IntentPrivate::getPrivate(m_intent).putArgumentExtras(argc, argv);
}

ApplicationContext::~ApplicationContext()
{
}

void ApplicationContext::setApplication(std::passed_ptr<Context> application)
{
    assert(content::ContextPrivate::getPrivate(*application).isActivity());

    if (!m_process->process.load(m_moduleName)) {
        LOGA("Application initialization failed");
        return;
    }

    m_application = std::move(application);
}

void ApplicationContext::setApplication(Intent& intent, int32_t action, int32_t flags)
{
    m_moduleName = content::IntentPrivate::getPrivate(intent).getModuleName();
    m_packageName = intent.getComponent().getPackageName();
    m_intent = intent;

    if (!m_process->process.load(m_moduleName)) {
        LOGA("Application initialization failed");
        return;
    }

    auto classInfo = ClassLoader::getSystemClassLoader().findClass(intent.getComponent().getClassName());
    if (!classInfo) {
        LOGA("Application class was not found");
        return;
    }

    auto application = classInfo->newInstance();
    m_application = std::static_pointer_cast<Context>(std::move(application));
    assert(content::ContextPrivate::getPrivate(*m_application).isApplication());
    m_action = action;
    m_flags = flags;
}

StringRef ApplicationContext::getModuleName()
{
    return m_moduleName;
}

Intent& ApplicationContext::getIntent()
{
    return m_intent;
}

int32_t ApplicationContext::getAction()
{
    return m_action;
}

int32_t ApplicationContext::getFlags()
{
    return m_flags;
}

std::passed_ptr<Bundle> ApplicationContext::getInstanceStateBundle()
{
    return m_stateBundle;
}

void ApplicationContext::setInstanceStateBundle(std::passed_ptr<Bundle> stateBundle)
{
    m_stateBundle = std::move(stateBundle);
}

std::shared_ptr<IBinder> ApplicationContext::getWindowToken()
{
    if (content::ContextPrivate::getPrivate(*m_application).isActivity()) {
        auto activity = std::static_pointer_cast<Activity>(m_application);
        return app::ActivityPrivate::getPrivate(*activity).hostWindow()->window()->getWindowToken();
    } else if (content::ContextPrivate::getPrivate(*m_application).isService()) {
        auto service = std::static_pointer_cast<Service>(m_application);
        return app::ServicePrivate::getPrivate(*service).hostWindow()->window()->getWindowToken();
    }

    return nullptr;
}

int32_t ApplicationContext::runApplication()
{
    Looper::prepareMainLooper();
    Looper::getMainLooper()->loop();
    return 0;
}

Context& ApplicationContext::getApplicationContext()
{
    return *this;
}

StringRef ApplicationContext::getPackageName()
{
    return m_packageName;
}

std::shared_ptr<Object> ApplicationContext::getSystemService(StringRef name)
{
    if (name.compare(Context::INPUT_METHOD_SERVICE) == 0) {
        static std::shared_ptr<InputMethodManager> imm = std::make_shared<InputMethodManager>();
        return imm;
    }

    return nullptr;
}

Resources& ApplicationContext::getResources()
{
    return Resources::getSystem();
}

bool ApplicationContext::bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags)
{
    return ApplicationLauncher::get().bindService(service, conn, flags);
}

void ApplicationContext::unbindService(std::passed_ptr<ServiceConnection> conn)
{
    ApplicationLauncher::get().unbindService(conn);
}

} // namespace android
