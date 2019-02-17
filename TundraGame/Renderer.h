#pragma once

#include "SpriteAtlas.h"

#include "Game/StaticRenderPass.h"
#include "Game/MotionBlurRenderPass.h"

#include "Event/Event.h"
#include "DynamicVertexBuffer.h"

class Renderer
{
private:
    Renderer();

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

    void present_Before();
    void present_After();

public:
    DynamicVertexBuffer<vec2f, g_MaxVerticesCount> Position_VBO;
    DynamicVertexBuffer<Color_UV_Data, g_MaxVerticesCount> Color_UV_VBO;

public: // TODO: fix to private
    StaticRenderPass m_staticPass;
    MotionBlurRenderPass m_motionBlurPass;

    FrameBufferID m_FBO;

    IndexBufferID m_IBO;

    TextureID m_atlas_Texture;
    TextureID m_target_Texture;

    u16 m_clientIndexBuffer[g_MaxIndicesCount];

    i32 m_currentSpriteCount;

    EventListener m_windowResizedListener;
};
