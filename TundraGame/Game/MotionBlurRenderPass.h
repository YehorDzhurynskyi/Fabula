#pragma once

class MotionBlurRenderPass
{
public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    ProgramID m_program;

    ShaderLocationID m_positionLocation;

    ShaderLocationID m_blurVecLocation;
    ShaderLocationID m_blurVecOffsetLocation;
    ShaderLocationID m_screenSizeLocation;

    vec2f m_prevPlayerPosition;
};
