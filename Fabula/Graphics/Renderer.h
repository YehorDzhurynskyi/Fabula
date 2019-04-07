#pragma once

#include "DynamicVertexBuffer.h"
#include "DynamicIndexBuffer.h"

#include "Fabula/Assets/SpriteAtlas.h"

#include "Fabula/Game/StaticRenderPass.h"
#include "Fabula/Game/MotionBlurRenderPass.h"

#include "Fabula/Layer/Event/Event.h"
#include "Fabula/Layer/Node.h"

#include "Fabula/Library/Singleton.h"

class Renderer final : public Node, public Singleton<Renderer>
{
friend class Singleton<Renderer>;

private:
    static const size_t g_MaxVerticesCount = 512;
    static const size_t g_MaxIndicesCount = g_MaxVerticesCount * 1.5;

    Renderer();

public:
    struct Color_UV_Data
    {
        u32 ColorTint;
        vec2f UV;
    };
    static_assert(sizeof(Color_UV_Data) == 12, "Unexpected size of vertex struct");

public:
    bool init();
    void shutdown();

    void render(const SpriteURI uri, const Transform& transform, const u32 colorTint);
    void render(const AnimatedSpriteURI uri, const int frame, const Transform& transform, const u32 colorTint);
    void render(const vec2f uvOffset, const vec2f uvSize, const Transform& transform, const u32 colorTint);

    void present_Before();
    void present_After();

protected:
    void onConnect(Layer& layer) override;

public:
    DynamicVertexBuffer<vec2f, g_MaxVerticesCount> Position_VBO;
    DynamicVertexBuffer<Color_UV_Data, g_MaxVerticesCount> Color_UV_VBO;

public: // TODO: fix to private
    StaticRenderPass m_staticPass;
    MotionBlurRenderPass m_motionBlurPass;

    FrameBufferID m_FBO;

    TextureID m_atlas_Texture;
    TextureID m_target_Texture;

    DynamicIndexBuffer<u16, g_MaxIndicesCount> m_IBO;

    i32 m_currentSpriteCount;

    EventListener m_windowResizedListener;
};
