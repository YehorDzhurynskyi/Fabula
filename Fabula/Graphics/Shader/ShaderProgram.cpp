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

void ShaderProgram::Release()
{
    DeleteShaders();
    if (IsBuilt())
    {
        fblGLCall(glDeleteProgram(m_Program));
        m_Program = 0;
    }
}

ShaderProgram::~ShaderProgram()
{
    Release();
}

bool ShaderProgram::IsBuilt() const
{
    return m_Program > 0;
}

bool ShaderProgram::AttachVertexShader(const char* sourceCode)
{
    fblAssert(!IsBuilt(), "Shader shouldn't be built here");

    m_VertexShader = compile_shader(GL_VERTEX_SHADER, sourceCode);

    return m_VertexShader > 0;
}

bool ShaderProgram::AttachFragmentShader(const char* sourceCode)
{
    fblAssert(!IsBuilt(), "Shader shouldn't be built here");

    m_FragmentShader = compile_shader(GL_FRAGMENT_SHADER, sourceCode);

    return m_FragmentShader > 0;
}

bool ShaderProgram::Build()
{
    fblAssert(!IsBuilt(), "Shader shouldn't be built here");
    fblAssert(m_VertexShader > 0 && m_FragmentShader > 0, "Shaders should be compiled here");

    if (m_VertexShader == 0 || m_FragmentShader == 0)
    {
        return false;
    }

    m_Program = fblGLCall(glCreateProgram());
    fblGLCall(glAttachShader(m_Program, m_VertexShader));
    fblGLCall(glAttachShader(m_Program, m_FragmentShader));

    fblGLCall(glLinkProgram(m_Program));

    GLint isLinked = 0;
    fblGLCall(glGetProgramiv(m_Program, GL_LINK_STATUS, &isLinked));
    if (isLinked == GL_FALSE)
    {
        char log[4096];
        fblGLCall(glGetShaderInfoLog(m_Program, sizeof(log), nullptr, log));
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);

        DeleteShaders();

        fblGLCall(glDeleteProgram(m_Program));

        return false;
    }

    fblGLCall(glDetachShader(m_Program, m_VertexShader));
    fblGLCall(glDetachShader(m_Program, m_FragmentShader));

    DeleteShaders();

    return m_Program > 0;
}

void ShaderProgram::DeleteShaders()
{
    if (m_VertexShader > 0)
    {
        fblGLCall(glDeleteShader(m_VertexShader));
    }

    if (m_FragmentShader > 0)
    {
        fblGLCall(glDeleteShader(m_FragmentShader));
    }

    m_VertexShader = 0;
    m_FragmentShader = 0;
}

void ShaderProgram::Bind()
{
    fblAssert(IsBuilt(), "Shader should be built here");

    fblGLCall(glUseProgram(m_Program));
}

void ShaderProgram::Unbind()
{
    fblAssert(IsBuilt(), "Shader should be built here");

    fblGLCall(glUseProgram(0));
}

fblShaderLocationID ShaderProgram::GetAttributeLocation(const char* name) const
{
    return fblGLCall(glGetAttribLocation(m_Program, name));
}

fblShaderLocationID ShaderProgram::GetUniformLocation(const char* name) const
{
    return fblGLCall(glGetUniformLocation(m_Program, name));
}

}
