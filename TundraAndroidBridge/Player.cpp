#include "pch.h"

#include "Game/Player.h"
#include "Game/Game.h"

#include "Renderer.h"

namespace
{

const float g_SkewAngle = 30.0f;
const vec2f g_GravityForce = vec2f(0.0f, 2.0f);
const vec2f g_ConstantForce = sinf(to_radians(g_SkewAngle)) * g_GravityForce;

float foo(float x)
{
    return std::pow(M_E, 4.0f * (x - 1.0f));
}

}

Player::Player()
    : m_directionSwitchListener(this, EventType::Click, [this](const Event& event)
{
    assert(event.type() == EventType::Click);
    const ClickEvent& clickEvent = AS(const ClickEvent&, event);
    m_inertia = m_ownVelocity;
    m_ownVelocity.x = -1.0f * m_ownVelocity.x;
    m_inertiaDamping = 1.0f - m_inertiaDamping;
})
{}

void Player::update()
{
    m_velocity = m_inertiaDamping * m_inertia + (1.0f - m_inertiaDamping) * m_ownVelocity;
    const vec2f& dp = g_DeltaTime * (g_ConstantForce + m_velocity);
    Transform.Position += dp;
    DistanceCovered += dp.length();

#if 0
    const float desiredZoom = m_inertiaDamping > 0.4f ? 1.4f : 1.0f;
    Camera::get().Zoom += (desiredZoom - Camera::get().Zoom) * 0.05f / (g_DeltaTime / 0.0166666f);
#endif

    m_inertiaDamping *= m_friction / (g_DeltaTime / 0.0166666f);
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

    {
        static float timer;
        timer -= g_DeltaTime;

        if (timer <= 0.0f)
        {
            FOR(5)
            {
                Particle* particle = TrailParticles.push();

                float offsetx = (2.0f * rand01() - 1.0f) * 0.3f;
                float offsety = rand01() * -2.0f;

                u8 r = (u8)(rand() % 255);
                u8 g = (u8)(rand() % 255);
                u8 b = (u8)(rand() % 255);

                particle->Transform.Position = Transform.Position + vec2f(offsetx, offsety);
                particle->ColorTint = FBL_COLOR(r, g, b, 0xff);
                particle->Life = 1.0f;
                particle->Velocity = -m_velocity * 0.015f * rand01();
            }
            timer = 0.1f;
        }
    }

    for (auto& node : TrailParticles)
    {
        if (!node.InUse)
        {
            continue;
        }

        Particle& particle = node.Value;
        particle.Life -= g_DeltaTime * 2.0f;

        particle.Transform.Position += particle.Velocity * g_DeltaTime;
        particle.Transform.Size = vec2f(0.25f, 0.25f) * particle.Life + vec2f(0.05f, 0.05f);

        const u8 alpha = particle.Life * 255;
        particle.ColorTint &= 0x00ffffff;
        particle.ColorTint |= (alpha << 24);
    }

    TrailParticles.rescan();
}

void Player::render() const
{
    Renderer::get().render(AnimatedSpriteURI::Player,
                           m_currentFrame,
                           Camera::get().toNDCSpace(Transform),
                           FBL_WHITE_COLOR);
}
