#pragma once

class CircleCollider
{
public:
    CircleCollider(const Transform& transform);

    bool vs(const CircleCollider& rhs, vec2f& outNormal);

private:
    float m_radius;
    const Transform& m_transform;
};

