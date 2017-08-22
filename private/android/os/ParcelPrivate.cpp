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

#include "ParcelPrivate.h"

#include "PlatformHandle.h"
#include "ServiceObject.h"
#include <android++/LogHelper.h>

using namespace java::io;

namespace android {
namespace os {

ParcelPrivate::ParcelPrivate(Parcel& parcel)
    : ByteReader(this)
    , ByteWriter(this)
    , m_parcel(parcel)
{
}

ParcelPrivate::~ParcelPrivate()
{
    if (m_sent)
        return;

    for (auto handle : m_handles)
        PlatformHandle::close(handle);

    for (auto object : m_serviceObjects)
        object->deref();
}

void ParcelPrivate::initializeWithCopy(Parcel& parcel, int8_t* data, size_t length)
{
    std::vector<int8_t>& buffer = ParcelPrivate::getPrivate(parcel).m_buffer;
    buffer.insert(buffer.end(), data, data + length);
    ParcelPrivate::getPrivate(parcel).reset();
}

ParcelPrivate& ParcelPrivate::getPrivate(Parcel& parcel)
{
    return *parcel.m_private;
}

void ParcelPrivate::setPrivate(Parcel& parcel, std::unique_ptr<ParcelPrivate>&& parcelPrivate)
{
    parcel.m_private = std::move(parcelPrivate);
}

size_t ParcelPrivate::size() const
{
    return m_buffer.size();
}

int8_t* ParcelPrivate::data()
{
    return m_buffer.data();
}

void ParcelPrivate::reset()
{
    ByteReader::reset();
    ByteWriter::reset();
}

void ParcelPrivate::setSent()
{
    m_sent = true;
}

void ParcelPrivate::setOrigin(std::passed_ptr<Binder> binder)
{
    m_origin = binder;
}

std::shared_ptr<Binder> ParcelPrivate::getOrigin()
{
    return m_origin;
}

void ParcelPrivate::hold(intptr_t handle)
{
    m_handles.insert(handle);
}

void ParcelPrivate::hold(ServiceObject* object)
{
    object->ref();
    m_serviceObjects.insert(object);
}

void ParcelPrivate::resize(size_t newSize)
{
    m_buffer.resize(newSize);
}

} // namespace os
} // namespace android
