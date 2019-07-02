#pragma once

namespace fbl
{

class CircleCollider
{
public:
    CircleCollider(const Transform& transform);
    CircleCollider(const vec2f& position, const float radius);

    bool vs(const CircleCollider& rhs, vec2f& outNormal);

private:
    float m_radius;
    const vec2f& m_position;
};

}
