#include "pch.h"
#include "Game/TrailRenderPass.h"
#include "Game/Game.h"

#include "Renderer.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

namespace
{

const char* g_TrailVertexShaderSource = ""
"attribute vec2 a_position;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_Position = vec4(a_position, 0.0, 1.0);\n"
"}\n";

const char* g_TrailFragmentShaderSource = ""
#ifdef FBL_WIN32
"#version 140\n"
#endif
"precision mediump float;\n"
"\n"
"void main(void)\n"
"{\n"
"    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
"}\n";

}

bool TrailRenderPass::init()
{
    const ShaderID vertexShader = Renderer::compile_shader(GL_VERTEX_SHADER, g_TrailVertexShaderSource);
    const ShaderID fragmentShader = Renderer::compile_shader(GL_FRAGMENT_SHADER, g_TrailFragmentShaderSource);

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
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(m_program);
        return false;
    }

    glDetachShader(m_program, vertexShader);
    glDetachShader(m_program, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_positionLocation = glGetAttribLocation(m_program, "a_position");
    if (m_positionLocation < 0)
    {
        glDeleteProgram(m_program);
        return false;
    }

    return true;
}

void TrailRenderPass::shutdown()
{
    glDeleteProgram(m_program);
}

void TrailRenderPass::bind()
{
    {
        Renderer::get().Position_VBO.bind();
        glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
    }

    glUseProgram(m_program);

    glEnableVertexAttribArray(m_positionLocation);
}

void TrailRenderPass::unbind()
{
    glDisableVertexAttribArray(m_positionLocation);
}
