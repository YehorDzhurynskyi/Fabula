#include "pch.h"
#include "LayerStack.h"
#include "SDL.h"

void LayerStack::flushEvents()
{
    for (const auto& event : m_eventQueue)
    {
        for (auto& layer : m_layers)
        {
            layer.handleEvent(*event);
        }
    }

    m_eventQueue.clear();
}

void LayerStack::update()
{
    for (auto& layer : m_layers)
    {
        layer.update();
    }
    //game.update();
}

void LayerStack::render() const
{
    for (const auto& layer : m_layers)
    {
        layer.render();
    }
    //game.render();
}
