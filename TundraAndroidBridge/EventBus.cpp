#include "pch.h"
#include "Event/EventBus.h"

EventListener::EventListener(void* owner)
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

    std::vector<EventListener>& typeHandlers = EventBus::get().m_handlers[m_eventType];
    assert(std::find(typeHandlers.begin(), typeHandlers.end(), *this) == typeHandlers.end());

    typeHandlers.push_back(std::move(*this));
}

void EventListener::unbind()
{
    assert(isValid());

    std::vector<EventListener>& typeHandlers = EventBus::get().m_handlers[m_eventType];

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

EventBus::~EventBus()
{
    for (auto& [eventType, handlers] : m_handlers)
    {
        for (auto& handler : handlers)
        {
            handler.unbind();
        }
    }
}

void EventBus::flush()
{
    for (const auto& event : m_eventQueue)
    {
        std::vector<EventListener>& typeHandlers = m_handlers[event->type()];
        for (const auto& listener : typeHandlers)
        {
            listener(*event);
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
                     "Event `%s` handled by %i listeners",
                     event->name(),
                     typeHandlers.size());
    }

    m_eventQueue.clear();
}