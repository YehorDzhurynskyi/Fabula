#pragma once

class TrailRenderPass
{
private:
    const static i32 PlayerTrailSize = 8;

public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    ProgramID m_program;

    ShaderLocationID m_positionLocation;
};
