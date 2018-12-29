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

    static const float g_MinimumVisibleWorldHeight;

public:
    vec2f getScreenSize() const;
    vec2f getVisibleWorldBounds() const;

    Transform toWorldSpace(const Transform& screenSpace) const;
    Transform toScreenSpace(const Transform& worldSpace) const;
    Transform toNDCSpace(const Transform& worldSpace) const;

    vec2f Position;
    float Zoom = 1.0f;
};

