#pragma once

#include "Layer.h"

class ApplicationLayer final : public Layer
{
public:
    ApplicationLayer();

    void update() override;
    void render() const override;
};
