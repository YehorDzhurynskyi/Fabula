#include "pch.h"
#include "TextRenderer.h"

#include "Graphics/API/opengl.h"

namespace
{

const char* g_TextVertexShaderSource = ""
"attribute vec2 a_position;\n"
"attribute vec2 a_uvtex;\n"
"attribute vec4 a_color;\n"
"\n"
"varying vec2 v_uvtex;\n"
"varying vec4 v_color;\n"
"\n"
"void main(void)\n"
"{\n"
"    v_uvtex = a_uvtex;\n"
"    v_color = a_color;\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_TextFragmentShaderSource = ""
#ifdef FBL_PLATFORM_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"varying vec2 v_uvtex;\n"
"varying vec4 v_color;\n"
"\n"
"uniform sampler2D u_texture;\n"
"\n"
"void main(void)\n"
"{\n"
"    float alpha = texture2D(u_texture, v_uvtex).r;\n"
"    gl_FragColor = vec4(v_color.rgb, v_color.a * alpha);\n"
"}\n";

}

namespace fbl
{

bool TextRenderer::init()
{
    const bool vertexShaderAttached = m_program.attachVertexShader(g_TextVertexShaderSource);
    const bool fragmentShaderAttached = m_program.attachFragmentShader(g_TextFragmentShaderSource);

    assert(vertexShaderAttached && fragmentShaderAttached);
    if (!vertexShaderAttached || !fragmentShaderAttached)
    {
        return false;
    }

    //m_fontAtlas = texture_atlas_new(512, 512, 1);
    //m_font = texture_font_new_from_file(m_fontAtlas, 32, "Assets/Staatliches-Regular.ttf");

    //glGenTextures(1, &m_fontAtlas->id);
    //glBindTexture(GL_TEXTURE_2D, m_fontAtlas->id);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_fontAtlas->width, m_fontAtlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, m_fontAtlas->data);

    m_program.build();

    m_positionLocation = m_program.getAttributeLocation("a_position");
    m_uvLocation = m_program.getAttributeLocation("a_uvtex");
    m_colorLocation = m_program.getAttributeLocation("a_color");

    if (m_positionLocation < 0 ||
        m_uvLocation < 0 ||
        m_colorLocation < 0)
    {
        shutdown();
        return false;
    }

    return true;
}

void TextRenderer::shutdown()
{
    m_program.release();

    //glDeleteTextures(1, &m_fontAtlas->id);
    //m_fontAtlas->id = 0;

    //texture_font_delete(m_font);
    //texture_atlas_delete(m_fontAtlas);
}

void TextRenderer::render_Text(const char* text, const vec2f position)
{
#if 0
    vec2f pen = position;
    FOR(strlen(text))
    {
        texture_glyph_t* glyph = texture_font_get_glyph(m_font, text + index);
        if (glyph != nullptr)
        {
            float kerning = 0.0f;
            if (index > 0)
            {
                kerning = texture_glyph_get_kerning(glyph, text + index - 1);
            }

            pen.x += kerning / Camera::get().getScreenSize().x;
            int x0 = (int)(pen.x + glyph->offset_x / Camera::get().getScreenSize().x);
            int y0 = (int)(pen.y + glyph->offset_y / Camera::get().getScreenSize().y);
            int x1 = (int)(x0 + glyph->width / Camera::get().getScreenSize().x);
            int y1 = (int)(y0 - glyph->height / Camera::get().getScreenSize().y);
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;

            const i32 offset = m_VBO.size();
            m_IBO.push(0 + offset);
            m_IBO.push(1 + offset);
            m_IBO.push(2 + offset);
            m_IBO.push(0 + offset);
            m_IBO.push(2 + offset);
            m_IBO.push(3 + offset);

            {
                GlyphData& glyphData = m_VBO.push();
                glyphData.Position = vec2f(x0, y0);
                glyphData.UV = vec2f(s0, t0);
                glyphData.Color = FBL_COLOR_MAGENTA;
            }

            {
                GlyphData& glyphData = m_VBO.push();
                glyphData.Position = vec2f(x0, y1);
                glyphData.UV = vec2f(s0, t1);
                glyphData.Color = FBL_COLOR_MAGENTA;
            }

            {
                GlyphData& glyphData = m_VBO.push();
                glyphData.Position = vec2f(x1, y1);
                glyphData.UV = vec2f(s1, t1);
                glyphData.Color = FBL_COLOR_MAGENTA;
            }

            {
                GlyphData& glyphData = m_VBO.push();
                glyphData.Position = vec2f(x1, y0);
                glyphData.UV = vec2f(s1, t0);
                glyphData.Color = FBL_COLOR_MAGENTA;
            }

            pen.x += glyph->advance_x / Camera::get().getScreenSize().x;
        }
    }
#endif
}

void TextRenderer::present()
{
    m_VBO.flush();
    m_IBO.flush();

    //glBindTexture(GL_TEXTURE_2D, m_fontAtlas->id);

    {
        m_VBO.bind();
        FBL_GL_CALL(glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphData), (void*)offsetof(GlyphData, Position)));
        FBL_GL_CALL(glVertexAttribPointer(m_colorLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(GlyphData), (void*)offsetof(GlyphData, Color)));
        FBL_GL_CALL(glVertexAttribPointer(m_uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GlyphData), (void*)offsetof(GlyphData, UV)));
    }

    m_program.use();

    FBL_GL_CALL(glEnableVertexAttribArray(m_positionLocation));
    FBL_GL_CALL(glEnableVertexAttribArray(m_colorLocation));
    FBL_GL_CALL(glEnableVertexAttribArray(m_uvLocation));

    FBL_GL_CALL(glDrawElements(GL_TRIANGLES, m_IBO.size(), GL_UNSIGNED_SHORT, (void*)0));

    FBL_GL_CALL(glDisableVertexAttribArray(m_positionLocation));
    FBL_GL_CALL(glDisableVertexAttribArray(m_colorLocation));
    FBL_GL_CALL(glDisableVertexAttribArray(m_uvLocation));
}
#if 0
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
#endif

}
