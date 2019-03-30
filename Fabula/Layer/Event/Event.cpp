#include "pch.h"
#include "Event.h"
#include "Layer/Layer.h"

EventListener::EventListener()
    : m_masterLayer(nullptr)
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
        m_masterLayer != nullptr &&
        m_eventType != EventType::None;
}

void EventListener::on(EventType eventType, EventHandler handler)
{
    m_eventType = eventType;
    m_handler = handler;
}

void EventListener::bind(Layer& layer)
{
    assert(m_masterLayer == nullptr);
    m_masterLayer = &layer;

    assert(isValid());

    std::vector<EventListener*>& typeHandlers = m_masterLayer->m_handlers[m_eventType];
    assert(std::find(typeHandlers.begin(), typeHandlers.end(), this) == typeHandlers.end());

    typeHandlers.push_back(this);
}

void EventListener::unbind()
{
    assert(isValid());

    std::vector<EventListener*>& typeHandlers = m_masterLayer->m_handlers[m_eventType];

    auto listenerIt = std::find(typeHandlers.begin(), typeHandlers.end(), this);
    assert(listenerIt != typeHandlers.end());
    typeHandlers.erase(listenerIt);
}
#if 0
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

bool EventListener::operator==(const EventListener& rhs) const
{
    return
        m_owner == rhs.m_owner &&
        m_eventType == rhs.m_eventType;
}
#endif

bool EventListener::handle(const Event& event) const
{
    assert(isValid());
    assert(event.type() == m_eventType);

    return m_handler(event);
}
