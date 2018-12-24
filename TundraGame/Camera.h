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

    static const float VisibleWorldHeight;

public:
    vec2f getScreenSize() const;
    vec2f getVisibleWorldBounds() const;

    Transform toWorldSpace(const Transform& screenSpace) const;
    Transform toScreenSpace(const Transform& worldSpace) const;
};

