#include "pch.h"
#include "Event.h"
#include "Layer/Layer.h"

namespace fbl
{

EventTypeID EventTypeIDNone = 0;
EventTypeID g_TypeIDCounter = EventTypeIDNone;

EventTypeID IEvent::GetEventTypeID() const
{
    return m_TypeID;
}

EventListener::EventListener()
    : m_masterLayer(nullptr)
    , m_eventType(EventTypeIDNone)
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
        m_eventType != EventTypeIDNone;
}

void EventListener::on(EventTypeID eventType, EventHandler handler)
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

bool EventListener::handle(const IEvent& event) const
{
    assert(isValid());
    assert(event.GetEventTypeID() == m_eventType);

    return m_handler(event);
}

}
