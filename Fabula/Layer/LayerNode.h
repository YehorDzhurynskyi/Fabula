#pragma once

#include <assert.h>

class Layer;
class LayerNode
{
public:
    virtual ~LayerNode() = default;

    void connect(Layer& layer)
    {
        assert(m_masterLayer == nullptr && "Can't reconnect `Node`");
        m_masterLayer = &layer;
        onConnect(layer);
    }

protected:
    virtual void onConnect(Layer& layer) = 0;

private:
    Layer* m_masterLayer = nullptr;
};
