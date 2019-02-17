#pragma once

#include "Event/Event.h"
#include <map>
#include <vector>
#include "Node.h"

class Layer : private Node
{
    friend class EventListener;
public:
    Layer();
    Layer(const Layer& rhs) = delete;
    Layer& operator=(const Layer& rhs) = delete;
    Layer(Layer&& rhs) = default;
    Layer& operator=(Layer&& rhs) = default;
    ~Layer();

    virtual void update() = 0;
    virtual void render() const = 0;

    bool handleEvent(const Event& event);

    bool isActive() const;

    void show();
    void hide();

    void setOnShowCallback(std::function<void(Layer*)> callback);
    void setOnHideCallback(std::function<void(Layer*)> callback);

private:
    bool m_isActive;

    std::function<void(Layer*)> m_onShowCallback;
    std::function<void(Layer*)> m_onHideCallback;

    std::map<EventType, std::vector<EventListener*>> m_handlers;
};
