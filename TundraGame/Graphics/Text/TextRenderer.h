#pragma once

#include "Library/Singleton.h"
#include "freetypegl/freetype-gl.h"
#include "freetypegl/vertex-buffer.h"

class TextRenderer : public Singleton<TextRenderer>
{
    friend class Singleton<TextRenderer>;

private:
    TextRenderer();

public:
    ~TextRenderer();

public:
    void render_TextLeft(const char* text, const vec2f position, const float rHeight);
    void render_TextCenter(const char* text, const vec2f position, const float rHeight);
    void render_TextRight(const char* text, const vec2f position, const float rHeight);

private:
    void render_Text(const char* text, const vec2f position, const float rHeight);

    float calculateTextWidth(const char* text, const float rHeight) const;
    u32 parseColor(const char* start, const char* end) const;

private:
    texture_atlas_t* m_fontAtlas = nullptr;
    texture_font_t* m_font = nullptr;
    vertex_buffer_t* m_textVertexBuffer = nullptr;
};

