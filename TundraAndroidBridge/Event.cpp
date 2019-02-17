#include "pch.h"
#include "Event/Event.h"
#include "Singleton.h"
#include "Layer.h"
#include "Node.h"

EventListener::EventListener(Node* owner)
    : m_owner(owner)
    , m_eventType(EventType::None)
    , m_handler(nullptr)
{}

EventListener::~EventListener()
{
    if (isValid())
    {
        unbind();
    }
}

bool EventListener::isValid() const
{
    return
        m_handler &&
        m_owner != nullptr &&
        m_eventType != EventType::None;
}

void EventListener::bind(EventType eventType, EventHandler handler)
{
    m_eventType = eventType;
    m_handler = handler;

    assert(isValid());

    std::vector<EventListener>& typeHandlers = m_owner->getMasterLayer()->m_handlers[m_eventType];
    assert(std::find(typeHandlers.begin(), typeHandlers.end(), this) == typeHandlers.end());

    typeHandlers.push_back(this);
}

void EventListener::unbind()
{
    assert(isValid());

    std::vector<EventListener>& typeHandlers = m_owner->getMasterLayer()->m_handlers[m_eventType];

    auto listenerIt = std::find(typeHandlers.begin(), typeHandlers.end(), *this);
    assert(listenerIt != typeHandlers.end());
    typeHandlers.erase(listenerIt);
}

EventListener::EventListener(EventListener&& rhs)
{
    m_owner = std::move(rhs.m_owner);
    m_eventType = std::move(rhs.m_eventType);
    m_handler = std::move(rhs.m_handler);

    rhs.reset();

    assert(isValid() && !rhs.isValid());
}

EventListener& EventListener::operator=(EventListener&& rhs)
{
    m_owner = std::move(rhs.m_owner);
    m_eventType = std::move(rhs.m_eventType);
    m_handler = std::move(rhs.m_handler);

    rhs.reset();

    assert(isValid() && !rhs.isValid());

    return *this;
}

void EventListener::reset()
{
    m_owner = nullptr;
    m_eventType = EventType::None;
    m_handler = nullptr;
}

#if 0
bool EventListener::operator==(const EventListener& rhs) const
{
    return
        m_owner == rhs.m_owner &&
        m_eventType == rhs.m_eventType;
}
#endif

void EventListener::operator()(const Event& event) const
{
    assert(m_handler);
    assert(m_owner != nullptr);
    assert(m_eventType != EventType::None);
    assert(event.type() == m_eventType);

    m_handler(event);
}
