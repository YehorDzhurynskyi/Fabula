#pragma once

#include "Library/Singleton.h"
#include "freetypegl/freetype-gl.h"
#include "Graphics/Shader/ShaderProgram.h"
#include "Graphics/DynamicVertexBuffer.h"

class TextRenderer : public Singleton<TextRenderer>
{
public:
    bool init();
    void shutdown();

    void render_Text(const char* text, const vec2f position, const float scale);
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

private:
    ShaderProgram m_program;

    texture_atlas_t* m_fontAtlas = nullptr;
    texture_font_t* m_font = nullptr;
    DynamicVertexBuffer<GlyphData, 128> m_textVertexBuffer;
};
