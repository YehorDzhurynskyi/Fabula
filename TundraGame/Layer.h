#pragma once

#include "Event/Event.h"
#include <map>
#include <vector>
#include "Scene.h"

class Layer final
{
    friend class EventListener;
public:
    ~Layer();

    void update();
    void render() const;

    bool handleEvent(const Event& event);

private:
    std::vector<std::unique_ptr<Scene>> m_scenes;
    std::map<EventType, std::vector<EventListener>> m_handlers;
};
