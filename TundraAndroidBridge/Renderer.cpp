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

namespace
{

const char* g_BasicVertexShaderSource = ""
"attribute vec2 a_position;\n"
"attribute vec2 a_uvtex;\n"
"attribute vec4 a_color_tint;\n"
"\n"
"varying vec2 v_uvtex;\n"
"varying vec4 v_color_tint;\n"
"\n"
"void main(void)\n"
"{\n"
"    v_uvtex = a_uvtex;\n"
"    v_color_tint = a_color_tint;\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_BasicFragmentShaderSource = ""
#ifdef FBL_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"uniform sampler2D u_texture;\n"
"\n"
"varying vec2 v_uvtex;\n"
"varying vec4 v_color_tint;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = v_color_tint * texture2D(u_texture, v_uvtex);\n"
"}\n";

const char* g_MotionBlurVertexShaderSource = ""
"attribute vec2 a_position;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_MotionBlurFragmentShaderSource = ""
#ifdef FBL_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"uniform vec2 u_prevCameraPos;\n"
"uniform vec2 u_currentCameraPos;\n"
"uniform sampler2D u_inTexture;\n"
"\n"
"void main(void)\n"
"{\n"
"    vec2 blurVec = u_currentCameraPos - u_prevCameraPos;\n"
"    gl_FragColor = texture2D(u_inTexture, gl_Position.xy);\n"
"    for (int i = 1; i < 32; ++i)\n"
"    {\n"
"        vec2 offset = blurVec * (float(i) / float(32 - 1) - 0.5);\n"
"        gl_FragColor += texture2D(u_inTexture, gl_Position.xy + offset);\n"
"    }\n"
"    gl_FragColor /= float(32);\n"
"}\n";

}

bool Renderer::init()
{
    const u32 vertexShader = compileShader(GL_VERTEX_SHADER, g_BasicVertexShaderSource);
    const u32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, g_BasicFragmentShaderSource);

    assert(vertexShader != 0 && fragmentShader != 0);
    if (vertexShader == 0 || fragmentShader == 0)
    {
        return false;
    }

    m_program = glCreateProgram();
    glAttachShader(m_program, vertexShader);
    glAttachShader(m_program, fragmentShader);

    glLinkProgram(m_program);

    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        char log[4096];
        glGetShaderInfoLog(m_program, sizeof(log), nullptr, log);
        SDL_Log("%s\n", log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_program);
        return false;
    }

    glDetachShader(m_program, vertexShader);
    glDetachShader(m_program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_MaxIndicesCount * sizeof(u16), nullptr, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, g_MaxVerticesCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    const i32 positionLocation = glGetAttribLocation(m_program, "a_position");
    const i32 uvLocation = glGetAttribLocation(m_program, "a_uvtex");
    const i32 colorTintLocation = glGetAttribLocation(m_program, "a_color_tint");

    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    glVertexAttribPointer(colorTintLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, ColorTint));

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

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

    return true;
}

void Renderer::shutdown()
{
    glDeleteTextures(1, &m_texture);
    glDeleteBuffers(1, &m_vbo);
    glDeleteProgram(m_program);
}

u32 Renderer::compileShader(i32 shaderType, const char* sourceCode)
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
        SDL_Log("%s\n", log);
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

    Vertex* vertices = &m_clientVertexBuffer[offset];

    const float sx = transform.Size.x;
    const float sy = transform.Size.y;

    vertices[0].Position = vec2f(-sx, sy) + transform.Position;
    vertices[1].Position = vec2f(-sx, -sy) + transform.Position;
    vertices[2].Position = vec2f(sx, sy) + transform.Position;
    vertices[3].Position = vec2f(sx, -sy) + transform.Position;

    vertices[0].UV = uvSize * vec2f(0.0f, 0.0f) + uvOffset;
    vertices[1].UV = uvSize * vec2f(0.0f, 1.0f) + uvOffset;
    vertices[2].UV = uvSize * vec2f(1.0f, 0.0f) + uvOffset;
    vertices[3].UV = uvSize * vec2f(1.0f, 1.0f) + uvOffset;

    vertices[0].ColorTint = colorTint;
    vertices[1].ColorTint = colorTint;
    vertices[2].ColorTint = colorTint;
    vertices[3].ColorTint = colorTint;

    u16* indices = &m_clientIndexBuffer[m_currentSpriteCount * 6];
    indices[0] = 0 + offset;
    indices[1] = 1 + offset;
    indices[2] = 2 + offset;
    indices[3] = 2 + offset;
    indices[4] = 1 + offset;
    indices[5] = 3 + offset;

    ++m_currentSpriteCount;
}

void Renderer::present()
{
    static vec2f prevCameraPos = Camera::get().Position;

    assert(m_currentSpriteCount <= g_MaxVerticesCount / 4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glUseProgram(m_program);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_currentSpriteCount * 6 * sizeof(u16), (void*)m_clientIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_currentSpriteCount * 4 * sizeof(Vertex), (void*)m_clientVertexBuffer);

    const i32 prevCameraPosLocation = glGetUniformLocation(m_program, "u_prevCameraPos");
    glUniform2f(prevCameraPosLocation, prevCameraPos.x, prevCameraPos.y);
    const i32 currentCameraPosLocation = glGetUniformLocation(m_program, "u_currentCameraPos");
    glUniform2f(currentCameraPosLocation, Camera::get().Position.x, Camera::get().Position.y);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    m_currentSpriteCount = 0;

    prevCameraPos = Camera::get().Position;
}

void Renderer::renderText(const char* text, const vec2f position, const float rHeight)
{
    assert(rHeight >= 0.0f && rHeight <= 1.0f);

    const vec2f screenSize = Camera::get().getScreenSize();
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

void Renderer::renderTextLeft(const char* text, const vec2f position, const float rHeight)
{
    return renderText(text, position, rHeight);
}

void Renderer::renderTextRight(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight);

    return renderText(text, newPos, rHeight);
}

void Renderer::renderTextCenter(const char* text, const vec2f position, const float rHeight)
{
    vec2f newPos(position);
    newPos.x -= calculateTextWidth(text, rHeight) * 0.5f;

    return renderText(text, newPos, rHeight);
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
