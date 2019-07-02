#pragma once

class ShaderProgram final
{
private:
    static ShaderID compile_shader(i32 shaderType, const char* sourceCode);

public:
    ~ShaderProgram();

    void release();

    bool isBuilt() const;

    bool attachVertexShader(const char* sourceCode);
    bool attachFragmentShader(const char* sourceCode);

    bool build();

    ShaderLocationID getAttributeLocation(const char* name) const;
    ShaderLocationID getUniformLocation(const char* name) const;

    void use() const;

private:
    void deleteShaders();

private:
    ProgramID m_program = 0;

    ShaderID m_vertexShader = 0;
    ShaderID m_fragmentShader = 0;
};
