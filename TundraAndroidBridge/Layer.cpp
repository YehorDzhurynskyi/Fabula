#include "pch.h"
#include "Layer.h"

Layer::Layer()
    : m_isActive(false)
{}

Layer::~Layer()
{
    for (auto&[eventType, handlers] : m_handlers)
    {
        for (auto& handler : handlers)
        {
            handler.unbind();
        }
    }
}

bool Layer::handleEvent(const Event& event)
{
    std::vector<EventListener>& typeHandlers = m_handlers[event.type()];
    for (const auto& listener : typeHandlers)
    {
        listener(event);
    }

    SDL_LogDebug(SDL_LOG_CATEGORY_INPUT,
                 "Event `%s` handled by %i listeners",
                 event.name(),
                 typeHandlers.size());

    return true;
}

bool Layer::isActive() const
{
    return m_isActive;
}

void Layer::show()
{
    assert(!m_isActive);

    m_isActive = true;
    if (m_onShowCallback)
    {
        m_onShowCallback(this);
    }
}

void Layer::hide()
{
    assert(m_isActive);

    m_isActive = false;
    if (m_onHideCallback)
    {
        m_onHideCallback(this);
    }
}

void Layer::setOnShowCallback(std::function<void(Layer*)> callback)
{
    m_onShowCallback = callback;
}

void Layer::setOnHideCallback(std::function<void(Layer*)> callback)
{
    m_onHideCallback = callback;
}
