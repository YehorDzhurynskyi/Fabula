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
        vec2f Position;
        vec2f UV;
        u32 ColorTint;
    };
    static_assert(sizeof(Vertex) == 20, "Unexpected size of vertex struct");

public:
    bool init();
    void shutdown();

    void render(const SpriteURI uri, const Transform& transform, const u32 colorTint);
    void render(const AnimatedSpriteURI uri, const int frame, const Transform& transform, const u32 colorTint);
    void render(const vec2f uvOffset, const vec2f uvSize, const Transform& transform, const u32 colorTint);

    void renderTextLeft(const char* text, const vec2f position, const float rHeight);
    void renderTextCenter(const char* text, const vec2f position, const float rHeight);
    void renderTextRight(const char* text, const vec2f position, const float rHeight);

    void present();

private:
    u32 compileShader(i32 shaderType, const char* sourceCode);
    void renderText(const char* text, const vec2f position, const float rHeight);
    float calculateTextWidth(const char* text, const float rHeight);

private:
    i32 m_currentSpriteCount = 0;

    Vertex m_clientVertexBuffer[g_MaxVerticesCount];
    u16 m_clientIndexBuffer[g_MaxIndicesCount];

    u32 m_program;
    u32 m_ibo;
    u32 m_vbo;
    u32 m_texture;
};
