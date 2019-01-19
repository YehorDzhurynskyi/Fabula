#pragma once

#include "SpriteAtlas.h"
#include "Game/BasicRenderPass.h"
#include "Game/MotionBlurRenderPass.h"

class Renderer
{
private:
    Renderer() = default;

    static const size_t g_MaxVerticesCount = 512;
    static const size_t g_MaxIndicesCount = g_MaxVerticesCount * 1.5;

public:
    static Renderer& get()
    {
        static Renderer instance;
        return instance;
    }

    struct Color_UV_Data
    {
        u32 ColorTint;
        vec2f UV;
    };
    static_assert(sizeof(Color_UV_Data) == 12, "Unexpected size of vertex struct");

    static u32 compile_shader(i32 shaderType, const char* sourceCode);

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
    void present2();

    u32 get_Position_VBO() const;
    u32 get_Color_UV_VBO() const;

private:
    void renderText(const char* text, const vec2f position, const float rHeight);
    float calculateTextWidth(const char* text, const float rHeight) const;
    u32 parseColor(const char* start, const char* end) const;

public:
    BasicRenderPass m_basicPass;
    MotionBlurRenderPass m_motionBlurPass;

    u32 m_FBO;

    u32 m_IBO;

    u32 m_position_VBO;
    u32 m_color_UV_VBO;

    u32 m_target_Texture;
    u32 m_atlas_Texture;

    i32 m_currentSpriteCount = 0;

    vec2f m_client_Position_VertexBuffer[g_MaxVerticesCount];
    Color_UV_Data m_client_Color_UV_VertexBuffer[g_MaxVerticesCount];

    u16 m_clientIndexBuffer[g_MaxIndicesCount];
};
