#pragma once

class MotionBlurRenderPass
{
public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    u32 m_program;

    i32 m_positionLocation;

    i32 m_blurVecLocation;
    i32 m_blurVecOffsetLocation;
    i32 m_screenSizeLocation;

    vec2f m_prevPlayerPosition;
};
