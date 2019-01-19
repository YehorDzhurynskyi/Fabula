#include "pch.h"
#include "Game/BasicRenderPass.h"

#include "SDL_image.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

#include "Renderer.h"

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

bool BasicRenderPass::init()
{
    const u32 vertexShader = Renderer::compileShader(GL_VERTEX_SHADER, g_BasicVertexShaderSource);
    const u32 fragmentShader = Renderer::compileShader(GL_FRAGMENT_SHADER, g_BasicFragmentShaderSource);

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

    const i32 positionLocation = glGetAttribLocation(m_program, "a_position");
    const i32 uvLocation = glGetAttribLocation(m_program, "a_uvtex");
    const i32 colorTintLocation = glGetAttribLocation(m_program, "a_color_tint");

    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
    glVertexAttribPointer(colorTintLocation, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, ColorTint));

    return true;
}

void BasicRenderPass::shutdown()
{
    glDeleteProgram(m_program);
}

void BasicRenderPass::present()
{
    static vec2f prevCameraPos = Camera::get().Position;

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

    prevCameraPos = Camera::get().Position;
}
