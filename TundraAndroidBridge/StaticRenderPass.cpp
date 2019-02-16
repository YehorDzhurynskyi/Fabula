#include "pch.h"
#include "Game/StaticRenderPass.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

#include "Camera.h"
#include "Renderer.h"

const char* g_StaticVertexShaderSource = ""
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

const char* g_StaticFragmentShaderSource = ""
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

bool StaticRenderPass::init()
{
    const ShaderID vertexShader = Renderer::compile_shader(GL_VERTEX_SHADER, g_StaticVertexShaderSource);
    const ShaderID fragmentShader = Renderer::compile_shader(GL_FRAGMENT_SHADER, g_StaticFragmentShaderSource);

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
    m_uvLocation = glGetAttribLocation(m_program, "a_uvtex");
    m_colorTintLocation = glGetAttribLocation(m_program, "a_color_tint");

    if (m_positionLocation < 0 ||
        m_uvLocation < 0 ||
        m_colorTintLocation < 0)
    {
        glDeleteProgram(m_program);
        return false;
    }

    return true;
}

void StaticRenderPass::shutdown()
{
    glDeleteProgram(m_program);
}

void StaticRenderPass::bind()
{
    {
        glBindBuffer(GL_ARRAY_BUFFER, Renderer::get().get_Position_VBO());
        glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
    }

    {
        glBindBuffer(GL_ARRAY_BUFFER, Renderer::get().get_Color_UV_VBO());
        glVertexAttribPointer(m_colorTintLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Renderer::Color_UV_Data), (void*)offsetof(Renderer::Color_UV_Data, ColorTint));
        glVertexAttribPointer(m_uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Renderer::Color_UV_Data), (void*)offsetof(Renderer::Color_UV_Data, UV));
    }

    glUseProgram(m_program);

    glEnableVertexAttribArray(m_positionLocation);
    glEnableVertexAttribArray(m_colorTintLocation);
    glEnableVertexAttribArray(m_uvLocation);
}

void StaticRenderPass::unbind()
{
    glDisableVertexAttribArray(m_positionLocation);
    glDisableVertexAttribArray(m_colorTintLocation);
    glDisableVertexAttribArray(m_uvLocation);
}
