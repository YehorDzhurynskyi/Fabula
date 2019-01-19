#include "pch.h"
#include "Game/MotionBlurRenderPass.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

#include "Renderer.h"
#include "Camera.h"

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
"uniform vec2 u_prevCameraPos;\n"
"uniform vec2 u_currentCameraPos;\n"
"uniform vec2 u_screenSize;\n"
"uniform sampler2D u_texture;\n"
"\n"
"void main(void)\n"
"{\n"
"    vec2 blurVec = u_currentCameraPos - u_prevCameraPos;\n"
"    blurVec *= 0.05;\n"
"    vec2 pixelUVCoord = gl_FragCoord.xy / u_screenSize;\n"
"    gl_FragColor = texture2D(u_texture, pixelUVCoord);\n"
"    int nOfSamples = 16;\n"
"    for (int i = 1; i < nOfSamples; ++i)\n"
"    {\n"
"        vec2 offset = blurVec * (float(i) / float(nOfSamples - 1)) - blurVec * 0.75;\n"
"        gl_FragColor += texture2D(u_texture, pixelUVCoord + offset);\n"
"    }\n"
"    gl_FragColor /= float(nOfSamples);\n"
"}\n";

}

bool MotionBlurRenderPass::init()
{
    const u32 vertexShader = Renderer::compile_shader(GL_VERTEX_SHADER, g_MotionBlurVertexShaderSource);
    const u32 fragmentShader = Renderer::compile_shader(GL_FRAGMENT_SHADER, g_MotionBlurFragmentShaderSource);

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

    m_positionLocation = glGetAttribLocation(m_program, "a_position");

    m_prevCameraPosLocation = glGetUniformLocation(m_program, "u_prevCameraPos");
    m_currentCameraPosLocation = glGetUniformLocation(m_program, "u_currentCameraPos");
    m_screenSizeLocation = glGetUniformLocation(m_program, "u_screenSize");

    if (m_positionLocation < 0 ||
        m_prevCameraPosLocation < 0 ||
        m_currentCameraPosLocation < 0 ||
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
        glBindBuffer(GL_ARRAY_BUFFER, Renderer::get().get_Position_VBO());

        glVertexAttribPointer(m_positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(vec2f), (void*)0);
    }

    glUseProgram(m_program);

    {
        const vec2f& currentCameraPosition = Camera::get().Position;
        const vec2f& screenSize = Camera::get().getScreenSize();

        glUniform2f(m_prevCameraPosLocation, m_prevCameraPosition.x, m_prevCameraPosition.y);
        glUniform2f(m_currentCameraPosLocation, currentCameraPosition.x, currentCameraPosition.y);
        glUniform2f(m_screenSizeLocation, screenSize.x, screenSize.y);

        m_prevCameraPosition = currentCameraPosition;
    }

    glEnableVertexAttribArray(m_positionLocation);
}

void MotionBlurRenderPass::unbind()
{
    glDisableVertexAttribArray(m_positionLocation);
}
