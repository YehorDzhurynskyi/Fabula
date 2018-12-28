#include "pch.h"
#include "Renderer.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

namespace
{

const char* g_VertexShaderSource = ""
"attribute vec2 a_vertex;\n"
"void main(void)\n"
"{\n"
"    gl_Position = vec4(a_vertex, 0.0, 1.0);\n"
"}\n";

const char* g_FragmentShaderSource = ""
"void main(void)\n"
"{\n"
"    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
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

    return true;
}

void Renderer::shutdown()
{
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

void Renderer::render()
{

}

