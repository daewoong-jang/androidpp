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

#include "ApplicationLoader.h"

#include <android/content/ApplicationLauncher.h>
#include <android/content/ContextPrivate.h>
#include <android/content/IntentPrivate.h>
#include <android/os/Looper.h>
#include <android/os/ParcelablePrivate.h>
#include <android++/LogHelper.h>
#include <thread>

namespace android {
namespace app {

ApplicationLoader::ApplicationLoader()
    : m_process(app::ApplicationProcess::current())
    , m_self(Binder::create(*this))
{
}

ApplicationLoader::~ApplicationLoader()
{
}

int32_t ApplicationLoader::start(intptr_t peer, StringRef component, std::unordered_map<String, String>& parameters)
{
    if (!m_process.initializeProcess(parameters)) {
        LOGA("Process initialization failed");
    }

    m_peer = Binder::adopt(peer);
    m_component = component;

    Looper::prepareMainLooper();

    Parcel parcel;
    writeResponseHeader(parcel);

    if (!m_peer->transact(ApplicationLauncher::ACK_APPLICATION_LOADER, parcel, nullptr, IBinder::FLAG_ONEWAY)) {
        LOGA("Connection to peer was broken before send back ACK_APPLICATION_LOADER");
        return -1;
    }

    Looper::getMainLooper()->loop();
    return 0;
}

void ApplicationLoader::sendOnBind(Service& service, std::passed_ptr<IBinder> binder)
{
    assert(m_state == BIND_SERVICE);

    Parcel parcel;
    writeResponseHeader(parcel);
    parcel << (binder ? std::static_pointer_cast<Binder>(binder)->handle() : 0);

    if (!m_peer->transact(ApplicationLauncher::SERVICE_ON_BIND, parcel, nullptr, IBinder::FLAG_ONEWAY)) {
        LOGE("Connection to launcher is broken for peer %x after bound to service", m_peer->handle());
    }

    m_state = SERVICE_CONNECTED;
}

Context& ApplicationLoader::getApplicationContext()
{
    return *m_context;
}

StringRef ApplicationLoader::getPackageName()
{
    return m_context->getPackageName();
}

std::shared_ptr<Object> ApplicationLoader::getSystemService(StringRef name)
{
    return m_context->getSystemService(name);
}

Resources& ApplicationLoader::getResources()
{
    return m_context->getResources();
}

bool ApplicationLoader::bindService(Intent& service, std::passed_ptr<ServiceConnection> conn, int32_t flags)
{
    return m_context->bindService(service, conn, flags);
}

void ApplicationLoader::unbindService(std::passed_ptr<ServiceConnection> conn)
{
    m_context->unbindService(conn);
}

void ApplicationLoader::writeResponseHeader(Parcel& parcel)
{
    parcel << m_component;
    parcel << m_self->handle();
    parcel << System::getProcessId();
}

void ApplicationLoader::onCreate()
{
}

void ApplicationLoader::onDestroy()
{
}

void ApplicationLoader::onTimer()
{
    switch (m_state) {
    case INITIALIZE_APPLICATION: {
        if (!m_process.initializeApplication(content::IntentPrivate::getPrivate(m_intent).getModuleName())) {
            LOGA("Application initialization failed");
            break;
        }

        Parcel parcel;
        writeResponseHeader(parcel);

        if (!m_peer->transact(ApplicationLauncher::APPLICATION_INITIALIZED, parcel, nullptr, IBinder::FLAG_ONEWAY)) {
            LOGE("Connection to launcher is broken for peer %x after initialization", m_peer->handle());
            break;
        }
        break;
    }
    case BIND_SERVICE: {
        m_context->setApplication(m_intent, BIND_SERVICE, m_flags);
        return;
    }
    default:
        break;
    }

    m_state = NO_ACTION;
}

void ApplicationLoader::onTransaction(int32_t code, Parcel& data, Parcel* reply, int32_t flags)
{
    switch (code) {
    case INITIALIZE_APPLICATION: {
        auto parcelable = ParcelablePrivate::createFromParcel(data, ParcelableCreator::creator<Intent>().binaryName);
        if (!parcelable) {
            LOGA("Received data is not an Intent object");
            break;
        }

        m_intent = *std::static_pointer_cast<Intent>(parcelable);
        break;
    }
    case BIND_SERVICE: {
        int32_t flags;
        data >> flags;

        m_context = std::make_unique<ApplicationContext>();
        content::ContextPrivate::getPrivate(*m_context).setApplicationLoader(this);
        m_flags = flags;
        break;
    }
    default:
        LOGA("False transaction code %d for ApplicationLauncher", code);
        break;
    }

    m_state = code;
    m_self->start();
}

} // namespace app
} // namespace android
