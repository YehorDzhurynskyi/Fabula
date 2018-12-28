#include "pch.h"
#include "Game/FontRenderer.h"
#include "Camera.h"

#include <array>

namespace
{

FontRenderer::GlyphArray&& initGlyphs()
{
    Sprite glyph_0;
    glyph_0.Offset = vec2i(0, 0);
    glyph_0.Size = vec2i(88, 128);

    Sprite glyph_1;
    glyph_1.Offset = vec2i(88, 0);
    glyph_1.Size = vec2i(32, 128);

    Sprite glyph_2;
    glyph_2.Offset = vec2i(120, 0);
    glyph_2.Size = vec2i(80, 128);

    Sprite glyph_3;
    glyph_3.Offset = vec2i(200, 0);
    glyph_3.Size = vec2i(80, 128);

    Sprite glyph_4;
    glyph_4.Offset = vec2i(280, 0);
    glyph_4.Size = vec2i(84, 128);

    Sprite glyph_5;
    glyph_5.Offset = vec2i(364, 0);
    glyph_5.Size = vec2i(80, 128);

    Sprite glyph_6;
    glyph_6.Offset = vec2i(444, 0);
    glyph_6.Size = vec2i(80, 128);

    Sprite glyph_7;
    glyph_7.Offset = vec2i(524, 0);
    glyph_7.Size = vec2i(80, 128);

    Sprite glyph_8;
    glyph_8.Offset = vec2i(604, 0);
    glyph_8.Size = vec2i(80, 128);

    Sprite glyph_9;
    glyph_9.Offset = vec2i(684, 0);
    glyph_9.Size = vec2i(80, 128);

    Sprite glyph_M;
    glyph_M.Offset = vec2i(764, 0);
    glyph_M.Size = vec2i(86, 128);

    return FontRenderer::GlyphArray {
        glyph_0, glyph_1, glyph_2, glyph_3,
        glyph_4, glyph_5, glyph_6, glyph_7,
        glyph_8, glyph_9, glyph_M
    };
}

FontRenderer::GlyphArray g_Glyphs = initGlyphs();

}

FontRenderer::FontRenderer()
    : m_atlas("Assets/fontatlas.png")
{}

void FontRenderer::renderText(const char* text,
                              const vec2f rPosition,
                              const float rHeight,
                              TextAlignment aligment)
{
    assert(rHeight >= 0.0f && rHeight <= 1.0f);
    assert(rPosition.x >= -1.0f && rPosition.x <= 1.0f);
    assert(rPosition.y >= -1.0f && rPosition.y <= 1.0f);

    const vec2f screenSize = Camera::get().getScreenSize();
    const float textHeight = screenSize.y * rHeight;
    const vec2f pivotPosition = rPosition * screenSize * 0.5f + screenSize * 0.5f;

    const float totalWidth = 0.0f;
    for (const char* ch = text; ch != '\0'; ++ch)
    {
        const i32 glyphIndex = *ch - '0';

    }
}
