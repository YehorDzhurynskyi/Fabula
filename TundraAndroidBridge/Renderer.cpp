#include "pch.h"
#include "Renderer.h"
#include "Camera.h"

#include "SDL_image.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

bool Renderer::init()
{
    bool initialized = m_staticPass.init() && m_motionBlurPass.init();
    if (!initialized)
    {
        return false;
    }

    { // Texture Atlas
        glGenTextures(1, &m_atlas_Texture);
        glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SDL_Surface* atlasSurface = IMG_Load("Assets/atlas.png");
        if (atlasSurface == nullptr)
        {
            REVEAL_SDL_ERROR("Failed to load sprite atlas")
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlasSurface->w, atlasSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, atlasSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        SDL_FreeSurface(atlasSurface);
    }

    { // IBO
        glGenBuffers(1, &m_IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_MaxIndicesCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);
    }

    { // Position VBO
        glGenBuffers(1, &m_position_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_position_VBO);
        glBufferData(GL_ARRAY_BUFFER, g_MaxVerticesCount * sizeof(vec2f), nullptr, GL_DYNAMIC_DRAW);
    }

    { // Color UV VBO
        glGenBuffers(1, &m_color_UV_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_color_UV_VBO);
        glBufferData(GL_ARRAY_BUFFER, g_MaxVerticesCount * sizeof(Color_UV_Data), nullptr, GL_DYNAMIC_DRAW);
    }

    { // FBO & Target Texture
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glGenTextures(1, &m_target_Texture);
        glBindTexture(GL_TEXTURE_2D, m_target_Texture);

        const vec2f screenSize = Camera::get().getScreenSize();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_target_Texture, 0);

        initialized = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    return initialized;
}

void Renderer::shutdown()
{
    glDeleteFramebuffers(1, &m_FBO);

    glDeleteBuffers(1, &m_IBO);

    glDeleteBuffers(1, &m_position_VBO);
    glDeleteBuffers(1, &m_color_UV_VBO);

    glDeleteTextures(1, &m_target_Texture);
    glDeleteTextures(1, &m_atlas_Texture);

    m_staticPass.shutdown();
    m_motionBlurPass.shutdown();
}

u32 Renderer::compile_shader(i32 shaderType, const char* sourceCode)
{
    const u32 shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCode, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        char log[4096];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Renderer::render(const SpriteURI uri, const Transform& transform, const u32 colorTint)
{
    render(SpriteAtlas::at(uri).Offset, SpriteAtlas::at(uri).Size, transform, colorTint);
}

void Renderer::render(const AnimatedSpriteURI uri, const int frame, const Transform& transform, const u32 colorTint)
{
    const AnimatedSprite& sprite = SpriteAtlas::at(uri);

    assert(frame == clamp<i32>(frame, 0, sprite.NOfFrames - 1));

    const float dU = sprite.Size.x / sprite.Pitch;
    const float dV = sprite.Size.y / (sprite.NOfFrames / sprite.Pitch);

    const float uOffset = sprite.Offset.x + dU * (frame % sprite.Pitch);
    const float vOffset = sprite.Offset.y + dV * (frame / sprite.Pitch);

    render(vec2f(uOffset, vOffset), vec2f(dU, dV), transform, colorTint);
}

void Renderer::render(const vec2f uvOffset, const vec2f uvSize, const Transform& transform, const u32 colorTint)
{
    const i32 offset = m_currentSpriteCount * 4;

    vec2f* positions = &m_client_Position_VertexBuffer[offset];
    Color_UV_Data* color_uv_data = &m_client_Color_UV_VertexBuffer[offset];

    const float sx = transform.Size.x;
    const float sy = transform.Size.y;

    positions[0] = vec2f(-sx, sy) + transform.Position;
    positions[1] = vec2f(-sx, -sy) + transform.Position;
    positions[2] = vec2f(sx, sy) + transform.Position;
    positions[3] = vec2f(sx, -sy) + transform.Position;

    color_uv_data[0].UV = uvSize * vec2f(0.0f, 0.0f) + uvOffset;
    color_uv_data[1].UV = uvSize * vec2f(0.0f, 1.0f) + uvOffset;
    color_uv_data[2].UV = uvSize * vec2f(1.0f, 0.0f) + uvOffset;
    color_uv_data[3].UV = uvSize * vec2f(1.0f, 1.0f) + uvOffset;

    color_uv_data[0].ColorTint = colorTint;
    color_uv_data[1].ColorTint = colorTint;
    color_uv_data[2].ColorTint = colorTint;
    color_uv_data[3].ColorTint = colorTint;

    u16* indices = &m_clientIndexBuffer[m_currentSpriteCount * 6];
    indices[0] = 0 + offset;
    indices[1] = 1 + offset;
    indices[2] = 2 + offset;
    indices[3] = 2 + offset;
    indices[4] = 1 + offset;
    indices[5] = 3 + offset;

    ++m_currentSpriteCount;
}

void Renderer::present_MotionBlured()
{
    present_Before();

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClear(GL_COLOR_BUFFER_BIT);

    {
        glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);
        m_staticPass.bind();

        glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

        m_staticPass.unbind();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    {
        glBindTexture(GL_TEXTURE_2D, m_target_Texture);
        m_motionBlurPass.bind();

        glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

        m_motionBlurPass.unbind();
    }

    present_After();
}

void Renderer::present_Static()
{
    present_Before();

    glBindTexture(GL_TEXTURE_2D, m_atlas_Texture);
    m_staticPass.bind();
    glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);
    m_staticPass.unbind();

    present_After();
}

void Renderer::present_Before()
{
    assert(m_currentSpriteCount <= g_MaxVerticesCount / 4);

    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_currentSpriteCount * 6 * sizeof(u16), (void*)m_clientIndexBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, Renderer::get().m_position_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Renderer::get().m_currentSpriteCount * 4 * sizeof(vec2f), (void*)Renderer::get().m_client_Position_VertexBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, Renderer::get().m_color_UV_VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, Renderer::get().m_currentSpriteCount * 4 * sizeof(Renderer::Color_UV_Data), (void*)Renderer::get().m_client_Color_UV_VertexBuffer);
    }
}

void Renderer::present_After()
{
    m_currentSpriteCount = 0;
}

void Renderer::render_Text(const char* text, const vec2f position, const float rHeight)
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

            render(glyphSprite->Offset, glyphSprite->Size, transform, color);
        }

        pivotPosition.x += rWidth * 2.0f;
    }
}

void Renderer::render_TextLeft(const char* text, const vec2f position, const float rHeight)
{
    return render_Text(text, position, rHeight);
}

void Renderer::render_TextRight(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight);

    return render_Text(text, newPos, rHeight);
}

void Renderer::render_TextCenter(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight) * 0.5f;

    return render_Text(text, newPos, rHeight);
}

float Renderer::calculateTextWidth(const char* text, const float rHeight) const
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

u32 Renderer::parseColor(const char* start, const char* end) const
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

u32 Renderer::get_Position_VBO() const
{
    return m_position_VBO;
}

u32 Renderer::get_Color_UV_VBO() const
{
    return m_color_UV_VBO;
}
