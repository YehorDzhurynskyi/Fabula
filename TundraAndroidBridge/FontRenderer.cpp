#include "pch.h"
#include "Game/FontRenderer.h"

#include "application.h"
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

void FontRenderer::renderText(const char* text, const vec2f position, const float rHeight)
{
    assert(rHeight >= 0.0f && rHeight <= 1.0f);

    const vec2f screenSize = Camera::get().getScreenSize();
    const float glyphHeight = screenSize.y * rHeight;
    vec2f pivotPosition = position;

    for (const char* ch = text; *ch != '\0'; ++ch)
    {
        const Sprite* glyphSprite = nullptr;
        switch (*ch)
        {
        case ' ':
        case 'm':
        case 'M':
        {
            glyphSprite = &g_Glyphs[AS(u8, GlyphURI::Glyph_M)];
        } break;
        default:
        {
            glyphSprite = &g_Glyphs[*ch - '0'];
        } break;
        }

        assert(glyphSprite != nullptr);
        const float rWidth = (glyphSprite->Size.x * rHeight) / glyphSprite->Size.y;
        const float glyphWidth = rWidth * screenSize.x;

        if (*ch != ' ')
        {
            SDL_Rect srcRect;
            srcRect.x = glyphSprite->Offset.x;
            srcRect.y = glyphSprite->Offset.y;
            srcRect.w = glyphSprite->Size.x;
            srcRect.h = glyphSprite->Size.y;

            SDL_Rect destRect;
            destRect.x = pivotPosition.x;
            destRect.y = pivotPosition.y;
            destRect.w = glyphWidth;
            destRect.h = glyphHeight;

#if 0
            const i32 res = SDL_RenderCopy(g_SDLRenderer,
                                           m_atlas.getSDLTexture(),
                                           &srcRect,
                                           &destRect);
            if (res < 0)
            {
                REVEAL_SDL_ERROR("SDL creating texture from surface failed")
            }
#endif
        }

        pivotPosition.x += glyphWidth;
    }
}

void FontRenderer::renderTextLeft(const char* text, const vec2f position, const float rHeight)
{
    return renderText(text, position, rHeight);
}

void FontRenderer::renderTextRight(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= Camera::get().getScreenSize().x * calculateTextRWidth(text, rHeight);

    return renderText(text, newPos, rHeight);
}

void FontRenderer::renderTextCenter(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= Camera::get().getScreenSize().x * calculateTextRWidth(text, rHeight) * 0.5f;

    return renderText(text, newPos, rHeight);
}

float FontRenderer::calculateTextRWidth(const char* text, const float rHeight)
{
    float rTotalWidth = 0.0f;
    for (const char* ch = text; *ch != '\0'; ++ch)
    {
        const Sprite* glyphSprite = nullptr;
        switch (*ch)
        {
        case ' ':
        case 'm':
        case 'M':
        {
            glyphSprite = &g_Glyphs[AS(u8, GlyphURI::Glyph_M)];
        } break;
        default:
        {
            glyphSprite = &g_Glyphs[*ch - '0'];
        } break;
        }

        assert(glyphSprite != nullptr);
        rTotalWidth += (glyphSprite->Size.x * rHeight) / glyphSprite->Size.y;
    }

    return rTotalWidth;
}
