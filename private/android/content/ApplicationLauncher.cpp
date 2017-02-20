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

#include "ApplicationLauncher.h"

#include <android/app/ApplicationLoader.h>
#include <android/content/ContextWrapper.h>
#include <android/content/Intent.h>
#include <android/content/ServiceConnection.h>
#include <android++/LogHelper.h>
#include <android++/StringConversion.h>

namespace android {
namespace content {

ApplicationLauncher::ApplicationLauncher()
    : m_self(Binder::create(*this))
{
}

ApplicationLauncher& ApplicationLauncher::get()
{
    static ApplicationLauncher* applicationLauncher = new ApplicationLauncher;
    return *applicationLauncher;
}

class ApplicationLauncher::Application {
public:
    String component;
    Intent intent;
    std::shared_ptr<ServiceConnection> connection;
    int32_t flags;
    intptr_t phandle { 0 };
    int64_t pid { 0 };
    std::shared_ptr<Binder> peer;
    int32_t state { 0 };
    int32_t command { 0 };
    std::shared_ptr<Binder> bind;

    Application(StringRef, Intent&, std::passed_ptr<ServiceConnection>, int32_t);

    bool launch();
};

ApplicationLauncher::Application::Application(StringRef component, Intent& intent, std::passed_ptr<ServiceConnection> conn, int32_t flags)
    : component(component)
    , intent(intent)
    , connection(std::move(conn))
    , flags(flags)
{
}

bool ApplicationLauncher::Application::launch()
{
    if (phandle)
        return true;

    phandle = ApplicationLauncher::get().platformCreateProcess(component);
    return !!phandle;
}

bool ApplicationLauncher::bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags)
{
    String key = service.getComponent().flattenToString();
    if (m_applications.count(key))
        return true;

    auto application = std::make_unique<Application>(key, service, conn, flags);
    if (!application->launch())
        return false;

    application->command = ApplicationLoader::BIND_SERVICE;
    m_applications[key] = std::move(application);
    return true;
}

void ApplicationLauncher::unbindService(std::passed_ptr<ServiceConnection> conn)
{
}

void ApplicationLauncher::onCreate()
{
}

void ApplicationLauncher::onDestroy()
{
}

void ApplicationLauncher::onTimer()
{
    for (auto& applicationKeyValue : m_applications) {
        auto& application = applicationKeyValue.second;
        switch (application->state) {
        case ACK_APPLICATION_LOADER: {
            Parcel parcel;
            application->intent.writeToParcel(parcel, Parcelable::PARCELABLE_WRITE_RETURN_VALUE);
            if (!application->peer->transact(ApplicationLoader::INITIALIZE_APPLICATION, parcel, nullptr, IBinder::FLAG_ONEWAY)) {
                LOGE("Connection to application is broken for peer %d:%x before initialization", application->pid, application->phandle);
                break;
            }
            break;
        }
        case APPLICATION_INITIALIZED: {
            switch (application->command) {
            case ApplicationLoader::BIND_SERVICE: {
                Parcel parcel;
                parcel << application->flags;
                if (!application->peer->transact(ApplicationLoader::BIND_SERVICE, parcel, nullptr, IBinder::FLAG_ONEWAY)) {
                    LOGE("Connection to application is broken for peer %d:%x before start", application->pid, application->phandle);
                    break;
                }
            }
            default:
                break;
            }
            break;
        }
        case SERVICE_ON_BIND: {
            if (application->connection) {
                application->connection->onServiceConnected(application->intent.getComponent(), application->bind);
            }
            break;
        }
        case APPLICATION_STARTED:
        default:
            break;
        }

        application->state = NO_ACTION;
    }
}

void ApplicationLauncher::onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    String component;
    intptr_t peer;
    int64_t pid;

    data >> component;
    data >> peer;
    data>> pid;

    if (!m_applications.count(component)) {
        LOGA("Unknown component: %s", std::ws2s(component).c_str());
        return;
    }

    auto& application = m_applications[component];

    switch (code) {
    case ACK_APPLICATION_LOADER: {
        application->pid = pid;
        application->peer = Binder::adopt(peer);
    }
    case APPLICATION_INITIALIZED:
    case APPLICATION_STARTED:
        break;
    case SERVICE_ON_BIND: {
        intptr_t handle;
        data >> handle;
        application->bind = handle ? Binder::adopt(handle) : nullptr;
        break;
    }
    default:
        LOGA("False transaction code %d for ApplicationLauncher", code);
        return;
    }

    application->state = code;
    m_self->start();
}

} // namespace content
} // namespace android
