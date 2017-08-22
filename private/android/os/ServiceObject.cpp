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

#include "ServiceObject.h"

#include "ParcelPrivate.h"
#include "ServiceChannel.h"
#include "ServiceObjectRef.h"
#include <android++/LogHelper.h>
#include <android++/NeverDestroyed.h>
#include <unordered_set>

namespace android {
namespace os {

struct ServiceObjectEntry {
    std::unique_ptr<ServiceObjectRef> reference;
    std::unordered_set<int64_t> clients;
};

static LazyNeverDestroyed<std::unordered_map<int32_t, ServiceObjectEntry>> s_registry;

static int32_t newObjectUid()
{
    static int32_t objectUid = 0;
    return ++objectUid;
}

ServiceObject::ServiceObject(ServiceChannel& channel)
    : m_channel(channel)
{
}

ServiceObject::~ServiceObject()
{
    LOGD("ServiceObject %d is being removed", m_objectUid);

    m_channel.remove(m_objectUid);
    s_registry->erase(m_objectUid);
}

std::shared_ptr<ServiceObject> ServiceObject::get(int32_t objectUid)
{
    if (s_registry->count(objectUid) == 0)
        return nullptr;

    return s_registry->at(objectUid).reference->ptr();
}

std::shared_ptr<ServiceObject> ServiceObject::add(std::shared_ptr<ServiceObject> object, CreationTag creation)
{
    if (!object)
        return nullptr;

    int32_t objectUid = object->objectUid();
    if (s_registry->count(objectUid) != 0)
        return object;

    LOGD("ServiceObject %d was %s process %lld", object->objectUid(), creation == Create ? "created in" : "imported to", System::getProcessId());

    s_registry->emplace(objectUid, ServiceObjectEntry { std::make_unique<ServiceObjectRef>(object) });

    object->m_channel.import(objectUid);
    return object;
}

void ServiceObject::ref()
{
    assert(s_registry->count(m_objectUid) != 0);
    s_registry->at(m_objectUid).reference->ref();
}

void ServiceObject::deref()
{
    assert(s_registry->count(m_objectUid) != 0);
    s_registry->at(m_objectUid).reference->deref();
}

void ServiceObject::createFromParcel(Parcel&)
{
    assert(ApplicationProcess::current().isRoot());
    m_objectUid = newObjectUid();
}

void ServiceObject::updateFromBundle(Bundle& data, int64_t senderPid)
{
    LOGD("ServiceObject %d is being updated by request from process %lld", m_objectUid, senderPid);
    assert(ApplicationProcess::current().isRoot());
}

void ServiceObject::readFromParcel(Parcel& source)
{
    source >> m_objectUid;
}

void ServiceObject::writeToParcel(Parcel& dest, int32_t flags) const
{
    LOGD("ServiceObject %d is being serialized in process %lld", m_objectUid, System::getProcessId());

    ParcelPrivate::getPrivate(dest).hold(const_cast<ServiceObject*>(this));

    dest << m_objectUid;
}

void ServiceObject::importToProcess(int64_t remotePid)
{
    LOGD("ServiceObject %d in process %lld was imported to process %lld", m_objectUid, System::getProcessId(), remotePid);

    assert(s_registry->count(m_objectUid) != 0);
    s_registry->at(m_objectUid).clients.insert(remotePid);
}

void ServiceObject::removedFromProcess(int64_t remotePid)
{
    LOGD("ServiceObject %d in process %lld was removed from process %lld", m_objectUid, System::getProcessId(), remotePid);

    assert(s_registry->count(m_objectUid) != 0);
    s_registry->at(m_objectUid).clients.erase(remotePid);
}

bool ServiceObject::update(Bundle& data)
{
    return m_channel.update(m_objectUid, data);
}

void ServiceObject::notify(int64_t senderPid)
{
    LOGD("ServiceObject %d is broadcasting update notification triggered by process %lld", m_objectUid, senderPid);
    assert(ApplicationProcess::current().isRoot());

    auto& clients = s_registry->at(m_objectUid).clients;
    for (auto client : clients) {
        if (client == senderPid)
            continue;
        if (!m_channel.notify(m_objectUid, senderPid, client))
            LOGE("Couldn't notify remote service object %d in process %lld", m_objectUid, client);
    }
}

} // namespace os
} // namespace android
