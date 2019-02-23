#include "pch.h"
#include "Event/EventBus.h"
#include "LayerStack.h"

void EventBus::flushEvents()
{
    for (const auto& event : m_eventQueue)
    {
        LayerStack::get().handleEvent(*event);
    }
    m_eventQueue.clear();
}
