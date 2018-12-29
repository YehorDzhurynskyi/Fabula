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
"\n"
"varying vec2 v_uvtex;\n"
"\n"
"void main(void)\n"
"{\n"
"    v_uvtex = a_uvtex;\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_FragmentShaderSource = ""
"uniform sampler2D t_texture;\n"
"varying vec2 v_uvtex;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = texture2D(t_texture, v_uvtex);\n"
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

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, g_VBOSize, nullptr, GL_DYNAMIC_DRAW);

    const i32 positionLocation = glGetAttribLocation(m_program, "a_position");
    const i32 uvLocation = glGetAttribLocation(m_program, "a_uvtex");

    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    //glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

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
    const Transform ndcTransform = Camera::get().toNDCSpace(transform);
    Vertex* vertices = &m_clientVertexBuffer[m_clientBufferVertexCount];

    const float sx = ndcTransform.size.x;
    const float sy = ndcTransform.size.y;

    vertices[0].position = vec2f(-sx, sy) + ndcTransform.position;
    vertices[1].position = vec2f(-sx, -sy) + ndcTransform.position;
    vertices[2].position = vec2f(sx, sy) + ndcTransform.position;
    vertices[3].position = vec2f(sx, -sy) + ndcTransform.position;

    vertices[0].uv = vec2f(0.0f, 0.0f);
    vertices[1].uv = vec2f(0.0f, 1.0f);
    vertices[2].uv = vec2f(1.0f, 0.0f);
    vertices[3].uv = vec2f(1.0f, 1.0f);

    m_clientBufferVertexCount += 4;
}

void Renderer::present()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glUseProgram(m_program);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    m_clientBufferVertexCount * sizeof(Vertex),
                    (void*)m_clientVertexBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, m_clientBufferVertexCount);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    m_clientBufferVertexCount = 0;
}
