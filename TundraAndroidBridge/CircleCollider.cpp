#include "pch.h"
#include "Game/CircleCollider.h"

CircleCollider::CircleCollider(const Transform& transform)
    : m_position(transform.Position)
{
    m_radius = 0.5f * std::max<float>(transform.Size.x, transform.Size.y);
}

CircleCollider::CircleCollider(const vec2f& position, const float radius)
    : m_position(position)
    , m_radius(radius)
{}

bool CircleCollider::vs(const CircleCollider& rhs, vec2f& outNormal)
{
    return false;
}
