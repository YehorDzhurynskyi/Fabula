#pragma once

class TrailRenderPass
{
public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    ProgramID m_program;


    ShaderLocationID m_positionLocation;
};
