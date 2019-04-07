#pragma once

#include "Event/Event.h"

class Layer
{
    friend class EventListener;
public:
    Layer() = default;
    Layer(const Layer& rhs) = delete;
    Layer& operator=(const Layer& rhs) = delete;
    Layer(Layer&& rhs) = default;
    Layer& operator=(Layer&& rhs) = default;
    virtual ~Layer();

    virtual void update() = 0;
    virtual void render() const = 0;

    bool handleEvent(const IEvent& event);

private:
    std::map<EventTypeID, std::vector<EventListener*>> m_handlers;
};
