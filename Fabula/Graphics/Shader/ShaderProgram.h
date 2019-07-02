#pragma once

namespace fbl
{

class ShaderProgram final
{
private:
    static fblShaderID compile_shader(fblS32 shaderType, const char* sourceCode);

public:
    ~ShaderProgram();

    void release();

    bool isBuilt() const;

    bool attachVertexShader(const char* sourceCode);
    bool attachFragmentShader(const char* sourceCode);

    bool build();

    fblShaderLocationID getAttributeLocation(const char* name) const;
    fblShaderLocationID getUniformLocation(const char* name) const;

    void use() const;

private:
    void deleteShaders();

private:
    fblProgramID m_program = 0;

    fblShaderID m_vertexShader = 0;
    fblShaderID m_fragmentShader = 0;
};

}

