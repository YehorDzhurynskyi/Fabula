#include "pch.h"
#include "ShaderProgram.h"

#include "Graphics/API/opengl.h"

ShaderID ShaderProgram::compile_shader(i32 shaderType, const char* sourceCode)
{
    const ShaderID shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &sourceCode, nullptr);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        char log[4096];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void ShaderProgram::release()
{
    deleteShaders();
    if (isBuilt())
    {
        glDeleteProgram(m_program);
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

    m_program = glCreateProgram();
    glAttachShader(m_program, m_vertexShader);
    glAttachShader(m_program, m_fragmentShader);

    glLinkProgram(m_program);

    GLint isLinked = 0;
    glGetProgramiv(m_program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE)
    {
        char log[4096];
        glGetShaderInfoLog(m_program, sizeof(log), nullptr, log);
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "%s\n", log);

        deleteShaders();

        glDeleteProgram(m_program);

        return false;
    }

    glDetachShader(m_program, m_vertexShader);
    glDetachShader(m_program, m_fragmentShader);

    deleteShaders();

    return m_program > 0;
}

void ShaderProgram::deleteShaders()
{
    if (m_vertexShader > 0)
    {
        glDeleteShader(m_vertexShader);
    }

    if (m_fragmentShader > 0)
    {
        glDeleteShader(m_fragmentShader);
    }

    m_vertexShader = 0;
    m_fragmentShader = 0;
}

void ShaderProgram::use() const
{
    assert(isBuilt());

    glUseProgram(m_program);
}

ShaderLocationID ShaderProgram::getAttributeLocation(const char* name) const
{
    return glGetAttribLocation(m_program, name);
}

ShaderLocationID ShaderProgram::getUniformLocation(const char* name) const
{
    return glGetUniformLocation(m_program, name);
}
