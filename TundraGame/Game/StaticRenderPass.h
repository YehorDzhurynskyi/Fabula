#pragma once

class StaticRenderPass
{
public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    ProgramID m_program;

    ShaderLocationID m_positionLocation;
    ShaderLocationID m_uvLocation;
    ShaderLocationID m_colorTintLocation;
};
