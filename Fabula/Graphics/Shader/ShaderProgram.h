#pragma once

namespace fbl
{

class ShaderProgram final
{
private:
    static fblShaderID compile_shader(fblS32 shaderType, const char* sourceCode);

public:
    ~ShaderProgram();

    void Release();

    bool IsBuilt() const;

    bool AttachVertexShader(const char* sourceCode);
    bool AttachFragmentShader(const char* sourceCode);

    bool Build();

    fblShaderLocationID GetAttributeLocation(const char* name) const;
    fblShaderLocationID GetUniformLocation(const char* name) const;

    void Bind();
    void Unbind();

private:
    void DeleteShaders();

private:
    fblProgramID m_Program = 0;

    fblShaderID m_VertexShader = 0;
    fblShaderID m_FragmentShader = 0;
};

}

