#pragma once

#include "Layer.h"

#include "Fabula/Library/Singleton.h"

namespace fbl
{

class LayerStack : public Singleton<LayerStack>
{
public:
    template<typename T, typename ... Args>
    T& push(Args&& ... args)
    {
        static_assert(std::is_base_of<Layer, T>::value, "Should derive from `Layer` class");
        m_layers.push_back(std::make_unique<T>(std::forward<Args>(args)...));

        Layer& layer = *m_layers.back().get();
        return AS(T&, layer);
    }
    void pop();

    void handleEvent(const IEvent& event);

    void update();
    void render() const;

private:
    std::vector<std::unique_ptr<Layer>> m_layers;
};

}
