#pragma once

class BasicRenderPass
{
public:
    bool init();
    void shutdown();

    void bind();
    void unbind();

private:
    u32 m_program;

    i32 m_positionLocation;
    i32 m_uvLocation;
    i32 m_colorTintLocation;
};
