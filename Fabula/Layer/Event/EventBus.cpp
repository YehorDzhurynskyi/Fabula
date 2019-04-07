#include "pch.h"
#include "EventBus.h"
#include "Layer/LayerStack.h"

namespace fbl
{

void EventBus::flushEvents()
{
    for (const auto& event : m_eventQueue)
    {
        LayerStack::get().handleEvent(*event);
    }
    m_eventQueue.clear();
}

}
