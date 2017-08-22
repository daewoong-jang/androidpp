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

#include "ByteReader.h"

namespace java {
namespace io {

ByteReader::ByteReader(ByteBufferProvider* buffer)
    : m_buffer(buffer)
{
}

ByteReader::~ByteReader()
{
}

void ByteReader::reset()
{
    m_pointer = m_buffer->data();
}

void ByteReader::read(void* out, size_t length, size_t alignment)
{
    if (!move(length, alignment))
        return;
    
    memcpy(out, m_pointer, length);
    m_pointer += length;
}

int8_t* ByteReader::readArray(size_t& length, size_t alignment)
{
    read(&length, sizeof(length), sizeof(length));
    if (!move(length * alignment, alignment))
        return nullptr;

    int8_t* arrayPosition = m_pointer;
    m_pointer += length * alignment;
    return arrayPosition;
}

static inline int8_t* alignPointer(int8_t* ptr, size_t alignment)
{
    uintptr_t alignmentMask = alignment - 1;
    return reinterpret_cast<int8_t*>((reinterpret_cast<intptr_t>(ptr) + alignmentMask) & ~alignmentMask);
}

static inline bool isAvailable(const int8_t* alignedPosition, const int8_t* bufferEnd, size_t size)
{
    return bufferEnd >= alignedPosition && static_cast<size_t>(bufferEnd - alignedPosition) >= size;
}

int8_t* ByteReader::move(size_t length, size_t alignment)
{
    int8_t* alignedPosition = alignPointer(m_pointer, alignment);
    if (!isAvailable(alignedPosition, m_buffer->data() + m_buffer->size(), length)) {
        return nullptr;
    }
    
    m_pointer = alignedPosition;
    return m_pointer;
}

} // namespace os
} // namespace android
