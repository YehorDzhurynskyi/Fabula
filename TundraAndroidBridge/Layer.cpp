#include "pch.h"
#include "Layer.h"

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

void Layer::update()
{
    for (auto& scene : m_scenes)
    {
        scene->update();
    }
}

void Layer::render() const
{
    for (const auto& scene : m_scenes)
    {
        scene->render();
    }
}
