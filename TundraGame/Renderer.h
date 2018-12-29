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
    static const size_t g_MaxVerticesCount = 512;
    static const size_t g_MaxIndicesCount = g_MaxVerticesCount * 1.5;

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
    i32 m_currentSpriteCount = 0;

    Vertex m_clientVertexBuffer[g_MaxVerticesCount];
    u16 m_clientIndexBuffer[g_MaxIndicesCount];

    u32 m_program;
    u32 m_ibo;
    u32 m_vbo;
    u32 m_texture;
};
