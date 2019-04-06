#include "pch.h"
#include "LayerStack.h"

void LayerStack::handleEvent(const Event& event)
{
    for (auto layerIt = m_layers.rbegin();
         layerIt != m_layers.rend();
         ++layerIt)
    {
        const bool needToPropagate = (*layerIt)->handleEvent(event);
        if (!needToPropagate)
        {
            break;
        }
    }
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
