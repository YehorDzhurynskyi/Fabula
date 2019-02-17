#pragma once

#include "Node.h"

class Scene : public Node
{
public:
    virtual void update() = 0;
    virtual void render() const = 0;

    virtual void onShow() = 0;
    virtual void onHide() = 0;
};
