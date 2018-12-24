#pragma once

class Camera
{
private:
    Camera() = default;

public:
    static Camera& get()
    {
        static Camera instance;
        return instance;
    }

public:
    void setViewPortSize(vec2i size);
    vec2i getViewPortSize() const;

private:
    vec2i m_viewportSize;
};

