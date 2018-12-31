#include "pch.h"
#include "Game/Player.h"
#include "Game/Game.h"

#include "Input.h"

namespace
{

const float g_SkewAngle = 30.0f;
const vec2f g_GravityForce = vec2f(0.0f, 2.0f);
const vec2f g_ConstantForce = sinf(to_radians(g_SkewAngle)) * g_GravityForce;

}

void Player::update()
{
    if (Input::DirectionSwitched)
    {
        m_inertia = m_ownVelocity;
        m_ownVelocity.x = -1.0f * m_ownVelocity.x;
        m_inertiaDamping = 1.0f - m_inertiaDamping;
    }

    const vec2f variableForce = m_inertiaDamping * m_inertia + (1.0f - m_inertiaDamping) * m_ownVelocity;
    Transform.Position += g_DeltaTime * (g_ConstantForce + variableForce);

    const float worldLimit = (Game::g_MapWidth - Transform.Size.x) * 0.5f;
    Transform.Position.x = clamp<float>(Transform.Position.x, -worldLimit, worldLimit);

    m_inertiaDamping *= (g_DeltaTime / 0.0166666f) * m_friction;
    if (m_inertiaDamping < 0.025f)
    {
        m_inertiaDamping = 0.0f;
    }
}
