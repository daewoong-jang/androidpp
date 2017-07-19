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

#include "Message.h"

#include <android/os/Binder.h>
#include <android/os/Bundle.h>
#include <android/os/BundlePrivate.h>
#include <android/os/Messenger.h>
#include <android/os/ParcelPrivate.h>
#include <android/os/ParcelablePrivate.h>

namespace android {
namespace os {

Message::Message()
    : what(0)
    , arg1(0)
    , arg2(0)
    , target(0)
    , replyTo(0)
    , data(nullptr)
{
}

Message::Message(const Message& o)
    : what(o.what)
    , arg1(o.arg1)
    , arg2(o.arg2)
    , obj(o.obj)
    , target(o.target)
    , replyTo(o.replyTo)
    , data((o.data) ? new Bundle(*o.data) : nullptr)
{
}

Message::Message(Message&& o)
    : what(o.what)
    , arg1(o.arg1)
    , arg2(o.arg2)
    , obj(o.obj)
    , target(o.target)
    , replyTo(o.replyTo)
    , data(o.data)
{
    o.replyTo = nullptr;
    o.data = nullptr;
}

Message& Message::operator=(const Message& other)
{
    what = other.what;
    arg1 = other.arg1;
    arg2 = other.arg2;
    obj = other.obj;
    target = other.target;
    replyTo = other.replyTo;
    data = (other.data) ? new Bundle(*other.data) : nullptr;
    return *this;
}

Message& Message::operator=(Message&& other)
{
    what = other.what;
    arg1 = other.arg1;
    arg2 = other.arg2;
    obj = other.obj;
    target = other.target;
    replyTo = other.replyTo;
    other.replyTo = nullptr;
    data = other.data;
    other.data = nullptr;
    return *this;
}

Message::~Message()
{
    if (data)
        delete data;
}

Message Message::obtain()
{
    return Message();
}

Message Message::obtain(Handler::ptr_t h)
{
    Message m;
    m.target = h;
    return m;
}

Message Message::obtain(Handler::ptr_t h, int32_t what)
{
    Message m;
    m.what = what;
    m.target = h;
    return m;
}

Message Message::obtain(Handler::ptr_t h, int32_t what, int32_t arg1, int32_t arg2)
{
    Message m;
    m.what = what;
    m.arg1 = arg1;
    m.arg2 = arg2;
    m.target = h;
    return m;
}

Message Message::obtain(Handler::ptr_t h, int32_t what, std::passed_ptr<Parcelable> obj)
{
    Message m;
    m.what = what;
    m.obj = obj;
    m.target = h;
    return m;
}

Message Message::obtain(Handler::ptr_t h, int32_t what, int32_t arg1, int32_t arg2, std::passed_ptr<Parcelable> obj)
{
    Message m;
    m.what = what;
    m.arg1 = arg1;
    m.arg2 = arg2;
    m.obj = obj;
    m.target = h;
    return m;
}

Message Message::obtain(const Message& orig)
{
    Message m;
    m.what = orig.what;
    m.arg1 = orig.arg1;
    m.arg2 = orig.arg2;
    m.obj = orig.obj;
    m.target = orig.target;
    m.replyTo = orig.replyTo;
    m.data = (orig.data) ? new Bundle(*orig.data) : nullptr;
    return m;
}

void Message::setData(Bundle& data)
{
    if (this->data)
        delete this->data;

    this->data = new Bundle(data);
}

void Message::setData(Bundle&& data)
{
    if (this->data)
        delete this->data;

    this->data = new Bundle(data);
}

Bundle& Message::getData()
{
    if (!data)
        data = new Bundle;

    return *data;
}

Bundle* Message::peekData()
{
    return data;
}

class MessageCreator final : public ParcelableCreator {
public:
    std::shared_ptr<Parcelable> createFromParcel(Parcel& source) override
    {
        auto result = std::make_shared<Message>();
        source >> result->what;
        source >> result->arg1;
        source >> result->arg2;
        bool hasObj;
        source >> hasObj;
        if (hasObj) {
            auto obj = ParcelablePrivate::createFromParcel(source);
            result->obj = BundlePrivate::getPrivate(result->getData()).setMessageObj(obj);
        }
        intptr_t handle;
        source >> handle;
        if (handle)
            result->replyTo = new Messenger(Binder::adopt(handle));
        bool hasData;
        source >> hasData;
        if (hasData) {
            String binaryName;
            source >> binaryName;
            assert(binaryName == ParcelableCreator::creator<Bundle>().binaryName);
            result->getData().readFromParcel(source);
        }
        return result;
    }

    std::vector<std::shared_ptr<Parcelable>> newArray(int32_t size) override
    {
        return {};
    }

    MessageCreator()
        : ParcelableCreator(this, L"android.os", L"Message.CREATOR")
    {
    }
};

const std::lazy_ptr<Parcelable::Creator> Message::CREATOR([] { return new MessageCreator; });

int32_t Message::describeContents()
{
    return 0;
}

void Message::writeToParcel(Parcel& dest, int32_t flags) const
{
    ParcelableCreator::writeToParcel(*this, dest);
    dest << what;
    dest << arg1;
    dest << arg2;
    dest << (obj ? true : false);
    if (obj)
        obj->writeToParcel(dest, flags);
    intptr_t handle = 0;
    if (replyTo) {
        auto binder = std::static_pointer_cast<Binder>(replyTo->getBinder());
        handle = binder->handle();
        ParcelPrivate::getPrivate(dest).setOrigin(binder);
    }
    dest << handle;
    bool hasData = (data && BundlePrivate::getPrivate(*data).count() > 0);
    dest << hasData;
    if (hasData)
        data->writeToParcel(dest, flags);
}

} // namespace os
} // namespace android
