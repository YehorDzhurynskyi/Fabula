#pragma once

#include "SpriteAtlas.h"

enum class TextAlignment : u8
{
    Left,
    Right,
    Center
};

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
    void renderText(const char* text, const vec2f rPosition, const float rHeight, TextAlignment aligment);

private:
    SpriteAtlas m_atlas;
};
