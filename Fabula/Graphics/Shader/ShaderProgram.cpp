#include "Fabula/pch.h"
#include "ShaderProgram.h"

#include "Fabula/Graphics/API/opengl.h"
#include <SDL.h>

namespace fbl
{

fblShaderID ShaderProgram::compile_shader(fblS32 shaderType, const char* sourceCode)
{
    const fblShaderID shader = fblGLCall(glCreateShader(shaderType));
    fblGLCall(glShaderSource(shader, 1, &sourceCode, nullptr));
    fblGLCall(glCompileShader(shader));

    GLint isCompiled = 0;
    fblGLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
    if (isCompiled == GL_FALSE)
    {
        char log[4096];
        fblGLCall(glGetShaderInfoLog(shader, sizeof(log), nullptr, log));
        fblLog("%s\n", log);
        fblGLCall(glDeleteShader(shader));
        return 0;
    }

    return shader;
}

void ShaderProgram::release()
{
    deleteShaders();
    if (isBuilt())
    {
        fblGLCall(glDeleteProgram(m_program));
        m_program = 0;
    }
}

ShaderProgram::~ShaderProgram()
{
    release();
}

bool ShaderProgram::isBuilt() const
{
    return m_program > 0;
}

bool ShaderProgram::attachVertexShader(const char* sourceCode)
{
    assert(!isBuilt());

    m_vertexShader = compile_shader(GL_VERTEX_SHADER, sourceCode);

    return m_vertexShader > 0;
}

bool ShaderProgram::attachFragmentShader(const char* sourceCode)
{
    assert(!isBuilt());

    m_fragmentShader = compile_shader(GL_FRAGMENT_SHADER, sourceCode);

    return m_fragmentShader > 0;
}

bool ShaderProgram::build()
{
    assert(!isBuilt());
    assert(m_vertexShader > 0 && m_fragmentShader > 0);

    if (m_vertexShader == 0 || m_fragmentShader == 0)
    {
        return false;
    }

    m_program = fblGLCall(glCreateProgram());
    fblGLCall(glAttachShader(m_program, m_vertexShader));
    fblGLCall(glAttachShader(m_program, m_fragmentShader));

    fblGLCall(glLinkProgram(m_program));

    GLint isLinked = 0;
    fblGLCall(glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked));
    if (isLinked == GL_FALSE)
    {
        char log[4096];
        fblGLCall(glGetShaderInfoLog(m_program, sizeof(log), nullptr, log));
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);

        deleteShaders();

        fblGLCall(glDeleteProgram(m_program));

        return false;
    }

    fblGLCall(glDetachShader(m_program, m_vertexShader));
    fblGLCall(glDetachShader(m_program, m_fragmentShader));

    deleteShaders();

    return m_program > 0;
}

void ShaderProgram::deleteShaders()
{
    if (m_vertexShader > 0)
    {
        fblGLCall(glDeleteShader(m_vertexShader));
    }

    if (m_fragmentShader > 0)
    {
        fblGLCall(glDeleteShader(m_fragmentShader));
    }

    m_vertexShader = 0;
    m_fragmentShader = 0;
}

void ShaderProgram::use() const
{
    assert(isBuilt());

    fblGLCall(glUseProgram(m_program));
}

fblShaderLocationID ShaderProgram::getAttributeLocation(const char* name) const
{
    return fblGLCall(glGetAttribLocation(m_program, name));
}

fblShaderLocationID ShaderProgram::getUniformLocation(const char* name) const
{
    return fblGLCall(glGetUniformLocation(m_program, name));
}

}
