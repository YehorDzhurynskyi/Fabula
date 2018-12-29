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

const char* g_VertexShaderSource = ""
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

const char* g_FragmentShaderSource = ""
#ifdef FBL_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"uniform sampler2D t_texture;\n"
"\n"
"varying vec2 v_uvtex;\n"
"varying vec4 v_color_tint;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = v_color_tint * texture2D(t_texture, v_uvtex);\n"
"}\n";

}

bool Renderer::init()
{
    const u32 vertexShader = compileShader(GL_VERTEX_SHADER, g_VertexShaderSource);
    const u32 fragmentShader = compileShader(GL_FRAGMENT_SHADER, g_FragmentShaderSource);

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

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 atlasSurface->w,
                 atlasSurface->h,
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 atlasSurface->pixels);
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

void Renderer::render(SpriteURI uri, const Transform& transform)
{
    render(uri, transform, FBL_COLOR(0xff, 0xff, 0xff, 0xff));
}

void Renderer::render(SpriteURI uri, const Transform& transform, const u32 colorTint)
{
    const Transform ndcTransform = Camera::get().toNDCSpace(transform);
    const i32 offset = m_currentSpriteCount * 4;

    Vertex* vertices = &m_clientVertexBuffer[offset];

    const float sx = ndcTransform.Size.x;
    const float sy = ndcTransform.Size.y;

    vertices[0].Position = vec2f(-sx, sy) + ndcTransform.Position;
    vertices[1].Position = vec2f(-sx, -sy) + ndcTransform.Position;
    vertices[2].Position = vec2f(sx, sy) + ndcTransform.Position;
    vertices[3].Position = vec2f(sx, -sy) + ndcTransform.Position;

    vertices[0].UV = vec2f(0.0f, 0.0f);
    vertices[1].UV = vec2f(0.0f, 1.0f);
    vertices[2].UV = vec2f(1.0f, 0.0f);
    vertices[3].UV = vec2f(1.0f, 1.0f);

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glUseProgram(m_program);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_currentSpriteCount * 6 * sizeof(u16), (void*)m_clientIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_currentSpriteCount * 4 * sizeof(Vertex), (void*)m_clientVertexBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawElements(GL_TRIANGLES, m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    m_currentSpriteCount = 0;
}
