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
};
