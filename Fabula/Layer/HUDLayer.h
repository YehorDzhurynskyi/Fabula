#pragma once

#include "Layer.h"
#include "Node.h"

class HUDLayer : public Layer, public Node
{
public:
    HUDLayer();

    void update() override;
    void render() const override;

protected:
    void onConnect(Layer& layer) override;

protected:
    EventListener m_clickEventListener;
};
