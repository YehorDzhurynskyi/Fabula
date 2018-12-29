#pragma once

#include "SpriteAtlas.h"

class Renderer
{
private:
    Renderer() = default;

public:
    static Renderer& get()
    {
        static Renderer instance;
        return instance;
    }

private:
    static const size_t g_VBOSize = 1024 * 8;

    struct Vertex
    {
        vec2f position;
        vec2f uv;
    };
    static_assert(sizeof(Vertex) == 16, "Unexpected size of vertex struct");

public:
    bool init();
    void shutdown();

    void render(SpriteURI uri, const Transform& transform);
    void present();

private:
    u32 compileShader(i32 shaderType, const char* sourceCode);

private:
    i32 m_clientBufferVertexCount = 0;
    Vertex m_clientVertexBuffer[g_VBOSize / sizeof(Vertex)];
    static_assert(ARRLEN(m_clientVertexBuffer) == 512, "Unexpected size of client vertex buffer");

    u32 m_program;
    u32 m_vbo;
    u32 m_texture;
};
