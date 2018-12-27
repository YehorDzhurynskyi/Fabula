#include "pch.h"
#include "Game/CircleCollider.h"

CircleCollider::CircleCollider(const Transform& transform)
    : m_transform(transform)
{
    m_radius = 0.5f * std::max<float>(m_transform.size.x, m_transform.size.y);
}

bool CircleCollider::vs(const CircleCollider& rhs, vec2f& outNormal)
{
    return false;
}
