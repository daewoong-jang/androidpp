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

#include "Handler.h"

#include "Looper.h"
#include "Message.h"
#include <android/os/HandlerProvider.h>
#include <java/lang/System.h>

#include <algorithm>
#include <mutex>
#include <android++/LogHelper.h>

namespace android {
namespace os {

class WorkItem {
public:
    WorkItem(Handler& h, std::chrono::milliseconds fireTime)
        : m_owner(h)
        , m_fireTime(fireTime)
    {
    }
    virtual ~WorkItem()
    {
    }

    virtual void performWork() = 0;

    virtual bool isMessageOf(int32_t what) = 0;
    virtual bool isMessageOf(std::function<void ()>& r) = 0;

    virtual std::chrono::milliseconds fireTime()
    {
        return m_fireTime;
    }

protected:
    Handler& m_owner;
    std::chrono::milliseconds m_fireTime;
};

class MessageWorkItem : public WorkItem {
public:
    MessageWorkItem(Handler& h, std::chrono::milliseconds fireTime, Message& msg)
        : WorkItem(h, fireTime)
        , m_message(msg)
    {
    }
    ~MessageWorkItem()
    {
    }

    void performWork()
    {
        m_owner.handleMessage(m_message);
        delete this;
    }

    bool isMessageOf(int32_t what)
    {
        return what == m_message.what;
    }
    bool isMessageOf(std::function<void ()>& r)
    {
        return false;
    }

private:
    Message m_message;
};

class RunnableWorkItem : public WorkItem {
public:
    RunnableWorkItem(Handler& h, std::chrono::milliseconds fireTime, std::function<void ()>&& r)
        : WorkItem(h, fireTime)
        , m_runnable(std::move(r))
    {
    }
    ~RunnableWorkItem()
    {
    }

    void performWork()
    {
        m_runnable();
        delete this;
    }

    bool isMessageOf(int32_t what)
    {
        return false;
    }
    bool isMessageOf(std::function<void ()>& r)
    {
        return m_runnable.target<void(*)()>() == r.target<void(*)()>();
    }

private:
    std::function<void ()> m_runnable;
};

static void enqueueWorkItem(std::vector<std::unique_ptr<WorkItem>>& workQueue, std::unique_ptr<WorkItem>&& item)
{
    workQueue.push_back(std::move(item));
    std::stable_sort(workQueue.begin(), workQueue.end(), [=] (const std::unique_ptr<WorkItem>& lhs, const std::unique_ptr<WorkItem>& rhs) {
        return lhs->fireTime() < rhs->fireTime();
    });
}

static const std::chrono::milliseconds oneMillisecond = std::chrono::duration<int32_t, std::milli>(1);

Handler::Handler()
    : m_handler(std::make_unique<HandlerProvider>(*this))
    , m_nextFireTime(std::chrono::milliseconds::max())
    , m_looper(Looper::myLooper())
{
}

Handler::~Handler()
{
}

Looper* Handler::getLooper()
{
    return m_looper;
}

void Handler::handleMessage(Message& msg)
{
}

bool Handler::hasMessages(int32_t what)
{
    synchronized (this) {
        for (auto& workItem : m_workQueue)
            if (workItem->isMessageOf(what))
                return true;
    }
    return false;
}

Message Handler::obtainMessage()
{
    return Message::obtain(this);
}

Message Handler::obtainMessage(int32_t what)
{
    return Message::obtain(this, what);
}

Message Handler::obtainMessage(int32_t what, int32_t arg1, int32_t arg2)
{
    return Message::obtain(this, what, arg1, arg2);
}

void Handler::removeCallbacks(std::function<void ()>&& r)
{
    removeWorkItems(std::move(r));
}

bool Handler::post(std::function<void ()>&& r)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<RunnableWorkItem>(*this, System::currentTimeMillis(), std::move(r)));
        return start();
    }
}

bool Handler::postAtFrontOfQueue(std::function<void ()>&& r)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<RunnableWorkItem>(*this, System::currentTimeMillis() - oneMillisecond, std::move(r)));
        return start();
    }
}

bool Handler::postAtTime(std::function<void ()>&& r, std::chrono::milliseconds uptimeMillis)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<RunnableWorkItem>(*this, uptimeMillis, std::move(r)));
        return startAtTime();
    }
}

bool Handler::postDelayed(std::function<void ()>&& r, std::chrono::milliseconds delayMillis)
{
    return postAtTime(std::move(r), System::currentTimeMillis() + delayMillis);
}

void Handler::removeMessages(int32_t what)
{
    removeWorkItems(what);
}

bool Handler::sendEmptyMessage(int32_t what)
{
    return sendMessage(obtainMessage(what));
}

bool Handler::sendEmptyMessageAtTime(int32_t what, std::chrono::milliseconds uptimeMillis)
{
    return sendMessageAtTime(obtainMessage(what), uptimeMillis);
}

bool Handler::sendEmptyMessageDelayed(int32_t what, std::chrono::milliseconds delayMillis)
{
    return sendMessageDelayed(obtainMessage(what), delayMillis);
}

bool Handler::sendMessage(Message& msg)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<MessageWorkItem>(*this, System::currentTimeMillis(), msg));
        return start();
    }
}

bool Handler::sendMessageAtFrontOfQueue(Message& msg)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<MessageWorkItem>(*this, System::currentTimeMillis() - oneMillisecond, msg));
        return start();
    }
}

bool Handler::sendMessageAtTime(Message& msg, std::chrono::milliseconds uptimeMillis)
{
    synchronized (this) {
        enqueueWorkItem(m_workQueue, std::make_unique<MessageWorkItem>(*this, uptimeMillis, msg));
        return startAtTime();
    }
}

bool Handler::sendMessageDelayed(Message& msg, std::chrono::milliseconds delayMillis)
{
    return sendMessageAtTime(msg, System::currentTimeMillis() + delayMillis);
}

template<typename... Arguments>
void Handler::removeWorkItems(Arguments... args)
{
    synchronized (this) {
        std::vector<int32_t> indices;

        int32_t i = 0;
        for (auto& workItem : m_workQueue) {
            if (workItem->isMessageOf(args...))
                indices.push_back(i);
            ++i;
        }

        stop();

        for (int32_t i = indices.size() - 1; i >= 0; --i)
            m_workQueue.erase(m_workQueue.begin() + indices[i]);

        startAtTime();
    }
}

bool Handler::start()
{
    stop();

    if (!m_workQueue.empty()) {
        m_nextFireTime = System::currentTimeMillis();
        return m_handler->start();
    }

    return false;
}

bool Handler::startAtTime()
{
    if (m_workQueue.empty())
        return false;

    std::chrono::milliseconds nextFireTime = m_workQueue[0]->fireTime();
    if (nextFireTime > m_nextFireTime)
        return true;

    m_nextFireTime = nextFireTime;
    return m_handler->startAtTime(nextFireTime);
}

void Handler::stop()
{
    m_nextFireTime = std::chrono::milliseconds::max();
    m_handler->stop();
}

void Handler::performMessages()
{
    if (m_workQueue.size() == 0)
        return;

    std::chrono::milliseconds currentTime = System::currentTimeMillis();
    std::vector<WorkItem*> firedItems;

    synchronized (this) {
        for (auto& workItem : m_workQueue) {
            if (workItem->fireTime() > currentTime)
                break;
            firedItems.push_back(workItem.release());
        }

        stop();

        if (firedItems.size() > 0)
            m_workQueue.erase(m_workQueue.begin(), m_workQueue.begin() + firedItems.size());

        startAtTime();
    }

    for (int32_t i = 0; i < firedItems.size(); i++)
        firedItems[i]->performWork();
}

void Handler::receivedMessage(Message& message)
{
    sendMessage(message);
}

} // namespace os
} // namespace android
