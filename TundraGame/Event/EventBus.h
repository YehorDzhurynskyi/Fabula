#pragma once

#include "Event.h"
#include <functional>
#include <vector>
#include <memory>
#include <map>

using EventHandler = std::function<void(const Event& event)>;
class EventListener
{
public:
    EventListener(void* owner, EventType eventType, EventHandler handler);
    ~EventListener();

    bool operator==(const EventListener& rhs) const;
    void operator()(const Event& event) const;

private:
    void* m_owner;
    EventType m_eventType;
    EventHandler m_handler;
};

class EventBus
{
    friend EventListener;
public:
    static EventBus& get()
    {
        static EventBus instance;
        return instance;
    }

private:
    EventBus() = default;

public:
    template<typename T, typename ...Args>
    T* enqueue(Args&& ...args)
    {
        static_assert(std::is_base_of<Event, T>::value, "should be derived from `Event`");
        m_eventQueue.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        T* t = static_cast<T*>(m_eventQueue.back().get());
        t->log();
        return t;
    }

    void flush();

private:
    std::vector<std::unique_ptr<Event>> m_eventQueue;
    std::map<EventType, std::vector<EventListener>> m_handlers;
};

