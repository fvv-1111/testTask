#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace Utils
{

template <typename TData>
class Queue
{
public:
    Queue() {};

    ~Queue() {};

    bool pop(TData& data)
    {
        std::lock_guard lock(m_mutex);
        bool ok = !m_queue.empty();
        if (ok)
        {
            data = std::move(m_queue.front());
            m_queue.pop();
        }
        return ok;
    }

    void push(const TData& data)
    {
        std::lock_guard lock(m_mutex);
        m_queue.push(data);
    }

    void push(TData&& data)
    {
        std::lock_guard lock(m_mutex);
        m_queue.push(std::move(data));
    }

    template<typename...T>
    void emplace(T&&...args)
    {
        std::lock_guard lock(m_mutex);
        m_queue.emplace(std::forward(args)...);
    }

private:
    std::mutex m_mutex;
    std::queue<TData> m_queue;

    // No copy
    Queue(const Queue&) = delete;
    void operator=(const Queue&) = delete;
    Queue(Queue&&) = delete;
    void operator = (Queue&&) = delete;

};

template <typename TData>
class QueueWithWaiting
{
public:

    QueueWithWaiting() {};

    ~QueueWithWaiting() {};

    // return: true - when data was read from the queue;
    //         false - when the queue is stopped and the queue is empty
    bool pop(TData& data)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_var.wait(lock, [self = this]() { return !self->m_queue.empty() || self->m_is_stopped; });
        if (!m_is_stopped && !m_queue.empty())
        {
            data = std::move(m_queue.front());
            m_queue.pop();
        }
        return !m_is_stopped;
    }

    void push(const TData& data)
    {
        std::lock_guard lock(m_mutex);
        if (!m_is_stopped)
        {
            m_queue.push(data);
            m_var.notify_one();
        }
    }

    void push(TData&& data)
    {
        std::lock_guard lock(m_mutex);
        if (!m_is_stopped)
        {
            m_queue.push(std::move(data));
            m_var.notify_one();
        }
    }

    template<typename...T>
    void emplace(T&&...args)
    {
        std::lock_guard lock(m_mutex);
        if (!m_is_stopped)
        {
            m_queue.emplace(std::forward<T>(args)...);
            m_var.notify_one();
        }
    }

    void stop()
    {
        std::lock_guard lock(m_mutex);
        m_is_stopped = true;
        m_var.notify_all();
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_var;
    std::queue<TData> m_queue;
    bool m_is_stopped = false;

    // No copy
    QueueWithWaiting(const QueueWithWaiting&) = delete;
    void operator=(const QueueWithWaiting&) = delete;
    QueueWithWaiting(QueueWithWaiting&&) = delete;
    void operator = (QueueWithWaiting&&) = delete;
};


};