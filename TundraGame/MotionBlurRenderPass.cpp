#include "pch.h"
#include "Game/MotionBlurRenderPass.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

#include "Renderer.h"
#include "Game/GameLayer.h"

namespace
{

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
"uniform vec2 u_blurVec;\n"
"uniform vec2 u_blurVecOffset;\n"
"uniform vec2 u_screenSize;\n"
"\n"
"uniform sampler2D u_texture;\n"
"\n"
"void main(void)\n"
"{\n"
"    vec2 pixelUVCoord = gl_FragCoord.xy / u_screenSize;\n"
"    gl_FragColor = texture2D(u_texture, pixelUVCoord);\n"
"    int nOfSamples = 8;\n"
"    for (int i = 1; i < nOfSamples; ++i)\n"
"    {\n"
"        vec2 offset = u_blurVec * (float(i) / float(nOfSamples - 1)) - u_blurVecOffset;\n"
"        gl_FragColor += texture2D(u_texture, pixelUVCoord + offset);\n"
"    }\n"
"    gl_FragColor /= float(nOfSamples);\n"
"}\n";

}

bool MotionBlurRenderPass::init()
{
    const ShaderID vertexShader = Renderer::compile_shader(GL_VERTEX_SHADER, g_MotionBlurVertexShaderSource);
    const ShaderID fragmentShader = Renderer::compile_shader(GL_FRAGMENT_SHADER, g_MotionBlurFragmentShaderSource);

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

    m_blurVecLocation = glGetUniformLocation(m_program, "u_blurVec");
    m_blurVecOffsetLocation = glGetUniformLocation(m_program, "u_blurVecOffset");
    m_screenSizeLocation = glGetUniformLocation(m_program, "u_screenSize");

    if (m_positionLocation < 0 ||
        m_blurVecLocation < 0 ||
        m_blurVecOffsetLocation < 0 ||
        m_screenSizeLocation < 0)
    {
        glDeleteProgram(m_program);
        return false;
    }

    return true;
}

void MotionBlurRenderPass::shutdown()
{
    glDeleteProgram(m_program);
}

void MotionBlurRenderPass::bind()
{
    {
        Renderer::get().Position_VBO.bind();
        glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
    }

    glUseProgram(m_program);

    {
        const vec2f& currentPlayerPosition = g_Game->getPlayer().Transform.Position;
        const vec2f& screenSize = Camera::get().getScreenSize();

        const vec2f& blurVec = (currentPlayerPosition - m_prevPlayerPosition) * 0.08f;
        const vec2f& blurVecOffset = blurVec * 0.75f;

        glUniform2f(m_blurVecLocation, -blurVec.x, blurVec.y);
        glUniform2f(m_blurVecOffsetLocation, -blurVecOffset.x, blurVecOffset.y);
        glUniform2f(m_screenSizeLocation, screenSize.x, screenSize.y);

        m_prevPlayerPosition = currentPlayerPosition;
    }

    glEnableVertexAttribArray(m_positionLocation);
}

void MotionBlurRenderPass::unbind()
{
    glDisableVertexAttribArray(m_positionLocation);
}
