#include "pch.h"
#include "Layer.h"

Layer::~Layer()
{
    for (auto&[eventType, handlers] : m_handlers)
    {
        for (auto& handler : handlers)
        {
            handler->unbind();
        }
    }
}

bool Layer::handleEvent(const IEvent& event)
{
    std::vector<EventListener*>& typeHandlers = m_handlers[event.GetEventTypeID()];

    bool needToPropagate = true;
    for (const auto& listener : typeHandlers)
    {
        needToPropagate = needToPropagate && listener->handle(event);
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
                 "Event `%s` handled by %i listeners",
                 event.GetName(),
                 typeHandlers.size());

    return needToPropagate;
}
