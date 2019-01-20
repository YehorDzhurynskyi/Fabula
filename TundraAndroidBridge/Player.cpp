#include "pch.h"
#include "Game/Player.h"
#include "Game/Game.h"

#include "Input.h"
#include "Renderer.h"

namespace
{

const float g_SkewAngle = 30.0f;
const vec2f g_GravityForce = vec2f(0.0f, 2.0f);
const vec2f g_ConstantForce = sinf(to_radians(g_SkewAngle)) * g_GravityForce;

}

float foo(float x)
{
    return std::pow(M_E, 4.0f * (x - 1.0f));
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

#if 0
    const float desiredZoom = m_inertiaDamping > 0.4f ? 1.4f : 1.0f;
    Camera::get().Zoom += (desiredZoom - Camera::get().Zoom) * 0.05f * (g_DeltaTime / 0.0166666f);
#endif

    m_inertiaDamping *= /*(g_DeltaTime / 0.0166666f) **/ m_friction;
    if (m_inertiaDamping < 0.0025f)
    {
        m_inertiaDamping = 0.0f;
    }

    const AnimatedSprite& sprite = SpriteAtlas::at(AnimatedSpriteURI::Player);
    m_currentFrame = m_inertiaDamping * (sprite.NOfFrames - 1);
    if (m_ownVelocity.x > 0.0f)
    {
        m_currentFrame = sprite.NOfFrames - 1 - m_currentFrame;
    }

    const float worldLimit = (Game::g_MapWidth - Transform.Size.x) * 0.5f;
    Transform.Position.x = clamp<float>(Transform.Position.x, -worldLimit, worldLimit);

    update_Trail();
}

void Player::update_Trail()
{
    FOR(2)
    {
        Particle* particle = m_trailParticles.push();
        particle->Color = index;
    }

    for (auto& node : m_trailParticles)
    {
        Particle& particle = node.Value;
        particle.Life -= g_DeltaTime;

        if (particle.Life > 0.0f)
        {
            particle.Position -= particle.Velocity * g_DeltaTime;
            u8 alpha = (particle.Color >> 24) & 0xff;
            alpha -= g_DeltaTime * 2.5f;
            particle.Color &= 0x00ffffff;
            particle.Color |= (alpha << 24);
        }
    }

    m_trailParticles.rescan();
}

void Player::render() const
{
    Renderer::get().render(AnimatedSpriteURI::Player,
                           m_currentFrame,
                           Camera::get().toNDCSpace(Transform),
                           FBL_WHITE_COLOR);
}
