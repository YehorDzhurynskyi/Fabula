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

    i32 m_prevCameraPosLocation;
    i32 m_currentCameraPosLocation;

    vec2f m_prevCameraPosition;
};
