#include "pch.h"
#include "Event/EventBus.h"

EventListener::EventListener(void* owner, EventType eventType, EventHandler handler)
    : m_owner(owner)
    , m_eventType(eventType)
    , m_handler(handler)
{
    assert(m_handler);
    assert(m_owner != nullptr);
    assert(m_eventType != EventType::None);

    std::vector<EventListener>& typeHandlers = EventBus::get().m_handlers[m_eventType];

    assert(std::find(typeHandlers.begin(), typeHandlers.end(), *this) == typeHandlers.end());

    typeHandlers.push_back(*this);
}

EventListener::~EventListener()
{
    assert(m_handler);
    assert(m_owner != nullptr);
    assert(m_eventType != EventType::None);

    std::vector<EventListener>& typeHandlers = EventBus::get().m_handlers[m_eventType];

    auto listenerIt = std::find(typeHandlers.begin(), typeHandlers.end(), *this);
    assert(listenerIt != typeHandlers.end());
    typeHandlers.erase(listenerIt);
}

bool EventListener::operator==(const EventListener& rhs) const
{
    return
        m_owner == rhs.m_owner &&
        m_eventType == rhs.m_eventType;
}

void EventListener::operator()(const Event& event) const
{
    assert(m_handler);
    assert(m_owner != nullptr);
    assert(m_eventType != EventType::None);
    assert(event.type() == m_eventType);

    m_handler(event);
}

void EventBus::flush()
{
    for (const auto& event : m_eventQueue)
    {
        event->log();
        std::vector<EventListener>& typeHandlers = m_handlers[event->type()];
        for (const auto& listener : typeHandlers)
        {
            listener(*event);
        }
    }

    m_eventQueue.clear();
}
