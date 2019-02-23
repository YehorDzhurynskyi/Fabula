#include "pch.h"
#include "TextRenderer.h"
#include "Renderer.h"

void TextRenderer::render_Text(const char* text, const vec2f position, const float rHeight)
{
    assert(rHeight >= 0.0f && rHeight <= 1.0f);

    vec2f pivotPosition = position;
    u32 color = FBL_BLACK_COLOR;

    for (const char* ch = text; ch != nullptr && *ch != '\0'; ++ch)
    {
        const Sprite* glyphSprite = nullptr;
        switch (*ch)
        {
        case '[':
        {
            const char* commandEnd = strchr(ch, ']');
            assert(commandEnd != nullptr);
            color = parseColor(ch + 1, commandEnd);
            ch = commandEnd;
            continue;
        } break;
        case ' ':
        case 'm':
        case 'M':
        {
            glyphSprite = &SpriteAtlas::at(SpriteURI::Glyph_M);
        } break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            glyphSprite = &SpriteAtlas::at(AS(SpriteURI, *ch - '0'));
        } break;
        default:
        {
            assert(!"Unrecognized symbol");
        } break;
        }

        assert(glyphSprite != nullptr);
        const float rWidth = (glyphSprite->Size.x * rHeight) / glyphSprite->Size.y;

        if (*ch != ' ')
        {
            Transform transform;
            transform.Position = pivotPosition;
            transform.Size = vec2f(rWidth, rHeight);

            Renderer::get().render(glyphSprite->Offset, glyphSprite->Size, transform, color);
        }

        pivotPosition.x += rWidth * 2.0f;
    }
}

void TextRenderer::render_TextLeft(const char* text, const vec2f position, const float rHeight)
{
    return render_Text(text, position, rHeight);
}

void TextRenderer::render_TextRight(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight);

    return render_Text(text, newPos, rHeight);
}

void TextRenderer::render_TextCenter(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight) * 0.5f;

    return render_Text(text, newPos, rHeight);
}

float TextRenderer::calculateTextWidth(const char* text, const float rHeight) const
{
    float rTotalWidth = 0.0f;
    for (const char* ch = text; ch != nullptr && *ch != '\0'; ++ch)
    {
        const Sprite* glyphSprite = nullptr;
        switch (*ch)
        {
        case '[':
        {
            ch = strchr(ch, ']');
            continue;
        } break;
        case ' ':
        case 'm':
        {
            glyphSprite = &SpriteAtlas::at(SpriteURI::Glyph_M);
        } break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            glyphSprite = &SpriteAtlas::at(AS(SpriteURI, *ch - '0'));
        } break;
        default:
        {
            assert(!"Unrecognized symbol");
        } break;
        }

        assert(glyphSprite != nullptr);
        rTotalWidth += (glyphSprite->Size.x * rHeight) / glyphSprite->Size.y;
    }

    return rTotalWidth;
}

u32 TextRenderer::parseColor(const char* start, const char* end) const
{
    u8 rgba[4] = { 0x0, 0x0, 0x0, 0xff };
    i32 channel = 0;

    while (start != end)
    {
        const char ch = *start;
        switch (ch)
        {
        case ' ':
        {
        } break;
        case ',':
        case '.':
        {
            channel = (channel + 1) % 4;
        } break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        {
            rgba[channel] *= 16;
            rgba[channel] += ch - '0';
        } break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        {
            rgba[channel] *= 16;
            rgba[channel] += ch - 'a' + 10;
        } break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        {
            rgba[channel] *= 16;
            rgba[channel] += ch - 'A' + 10;
        } break;
        default:
        {
            assert(!"Unrecognized symbol");
        } break;
        }
        ++start;
    }
    return FBL_COLOR(rgba[0], rgba[1], rgba[2], rgba[3]);
}
