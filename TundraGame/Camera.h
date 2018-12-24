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

    static const float DesiredVisibleWorldWidth;
    static const float MinimumWorldHeight;

public:
    vec2f getScreenSize() const;
    vec2f getVisibleWorldBounds() const;

    Transform toWorldSpace(const Transform& screenSpace) const;
    Transform toScreenSpace(const Transform& worldSpace) const;

private:
    float m_zoom = 1.0f;
};

