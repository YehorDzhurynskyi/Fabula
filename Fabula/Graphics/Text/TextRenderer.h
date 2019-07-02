#pragma once

#include "Fabula/Graphics/Shader/ShaderProgram.h"
#include "Fabula/Graphics/DynamicVertexBuffer.h"
#include "Fabula/Graphics/DynamicIndexBuffer.h"

#include "Fabula/Library/Singleton.h"

namespace fbl
{

class TextRenderer : public Singleton<TextRenderer>
{
public:
    bool init();
    void shutdown();

    void render_Text(const char* text, const vec2f position);

    void present();

#if 0
public:
    void render_TextLeft(const char* text, const vec2f position, const float rHeight);
    void render_TextCenter(const char* text, const vec2f position, const float rHeight);
    void render_TextRight(const char* text, const vec2f position, const float rHeight);

private:
    void render_Text(const char* text, const vec2f position, const float rHeight);

    float calculateTextWidth(const char* text, const float rHeight) const;
    u32 parseColor(const char* start, const char* end) const;
#endif

private:
    struct GlyphData
    {
        vec2f Position;
        vec2f UV;
        u32 Color;
    };

    static const i32 g_MaxGlyphsCount = 128;
    static const i32 g_MaxVerticesCount = g_MaxGlyphsCount * 4;
    static const i32 g_MaxIndicesCount = g_MaxVerticesCount * 6;

private:
    ShaderProgram m_program;

    ShaderLocationID m_positionLocation;
    ShaderLocationID m_uvLocation;
    ShaderLocationID m_colorLocation;

    DynamicVertexBuffer<GlyphData, g_MaxVerticesCount> m_VBO;
    DynamicIndexBuffer<u16, g_MaxIndicesCount> m_IBO;
};

}
