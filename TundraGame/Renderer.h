#pragma once

#include "SpriteAtlas.h"

class Renderer
{
private:
    Renderer() = default;

public:
    static Renderer get()
    {
        static Renderer instance;
        return instance;
    }

public:
    void init();
    void shutdown();

    void render();

};
