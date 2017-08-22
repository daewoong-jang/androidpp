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

#include "ByteWriter.h"

namespace java {
namespace io {

ByteWriter::ByteWriter(ByteBufferProvider* buffer)
    : m_buffer(buffer)
{
}

ByteWriter::~ByteWriter()
{
}

void ByteWriter::reset()
{
    m_pointer = m_buffer->data();
}

void ByteWriter::write(const void* in, size_t length, size_t alignment)
{
    int8_t* buffer = grow(length, alignment);
    memcpy(buffer, in, length);
}

void ByteWriter::writeArray(const void* in, size_t length, size_t alignment)
{
    write(&length, sizeof(length), sizeof(length));
    write(in, length * alignment, alignment);
}

static inline size_t alignLength(size_t length, size_t alignment)
{
    return ((length + alignment - 1) / alignment) * alignment;
}

int8_t* ByteWriter::grow(size_t length, size_t alignment)
{
    size_t alignedSize = alignLength(m_buffer->size(), alignment);
    m_buffer->resize(alignedSize + length);
    return m_buffer->data() + alignedSize;
}

} // namespace io
} // namespace java
