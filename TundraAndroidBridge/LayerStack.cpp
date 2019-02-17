#include "pch.h"
#include "LayerStack.h"

void LayerStack::flushEvents()
{
    for (const auto& event : m_eventQueue)
    {
        for (auto layerIt = m_layers.rbegin();
             layerIt != m_layers.rend();
             ++layerIt)
        {
            (*layerIt)->handleEvent(*event);
        }
    }

    m_eventQueue.clear();
}

void LayerStack::update()
{
    for (auto& layer : m_layers)
    {
        layer->update();
    }
}

void LayerStack::render() const
{
    for (const auto& layer : m_layers)
    {
        layer->render();
    }
}

void LayerStack::pop()
{
    assert(m_layers.size() > 1);

    m_layers.back()->hide();
}
