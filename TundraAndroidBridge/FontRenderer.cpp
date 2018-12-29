#include "pch.h"
#include "Game/FontRenderer.h"

#include "application.h"
#include "Camera.h"

#include <array>

#if 0
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
#endif
