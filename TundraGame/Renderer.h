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
    bool init();
    void shutdown();

    void render();

private:
    u32 compileShader(i32 shaderType, const char* sourceCode);

private:
    u32 m_program;
};
