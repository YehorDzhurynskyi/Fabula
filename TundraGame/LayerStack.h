#pragma once

#include "Singleton.h"
#include "Layer.h"
#include <vector>

class LayerStack : public Singleton<LayerStack>
{
public:
    template<typename T, typename ...Args>
    T* enqueueEvent(Args&& ...args)
    {
        static_assert(std::is_base_of<Event, T>::value, "should be derived from `Event`");
        m_eventQueue.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        return static_cast<T*>(m_eventQueue.back().get());
    }

    template<typename T, typename ... Args>
    T& push(Args&& ... args)
    {
        static_assert(std::is_base_of<Layer, T>::value, "Should derive from `Layer` class");
        m_layers.push_back(std::make_unique<T>(std::forward<Args>(args)...));

        Layer& layer = *m_layers.back().get();
        layer.show();

        return AS(T&, layer);
    }
    void pop();

    void flushEvents();

    void update();
    void render() const;

private:
    std::vector<std::unique_ptr<Layer>> m_layers;
    std::vector<std::unique_ptr<Event>> m_eventQueue;
};
