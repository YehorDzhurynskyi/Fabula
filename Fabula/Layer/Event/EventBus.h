#pragma once

#include "Event.h"

#include "Fabula/Library/Singleton.h"

class EventBus final : public Singleton<EventBus>
{
public:
    template<typename T, typename ...Args>
    T* enqueueEvent(Args&& ...args)
    {
        static_assert(std::is_base_of<Event, T>::value, "should be derived from `Event`");
        m_eventQueue.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return static_cast<T*>(m_eventQueue.back().get());
    }

    void flushEvents();

private:
    std::vector<std::unique_ptr<Event>> m_eventQueue;
};
