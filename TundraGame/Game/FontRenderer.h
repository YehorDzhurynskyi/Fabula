#pragma once

#include "SpriteAtlas.h"

class FontRenderer
{
private:
    enum class GlyphURI : u8
    {
        Glyph_0 = 0,
        Glyph_1,
        Glyph_2,
        Glyph_3,
        Glyph_4,
        Glyph_5,
        Glyph_6,
        Glyph_7,
        Glyph_8,
        Glyph_9,
        Glyph_M,
        COUNT
    };

    FontRenderer();

public:
    using GlyphArray = std::array<Sprite, AS(u8, FontRenderer::GlyphURI::COUNT)>;

    static FontRenderer& get()
    {
        static FontRenderer instance;
        return instance;
    }

public:
    void renderText(const char* text, const vec2f position, const float rHeight);
    void renderTextLeft(const char* text, const vec2f position, const float rHeight);
    void renderTextRight(const char* text, const vec2f position, const float rHeight);
    void renderTextCenter(const char* text, const vec2f position, const float rHeight);

private:
    float calculateTextRWidth(const char* text, const float rHeight);

private:
    SpriteAtlas m_atlas;
};
