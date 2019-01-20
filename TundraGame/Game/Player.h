#pragma once

#include "Pool.h"

struct Particle
{
    vec2f Position;
    vec2f Velocity;
    u32 Color;
    float Life;

    bool isAlive() const
    {
        return Life > 0.0f;
    }
};

static_assert(sizeof(Particle) == 24, "size should be 24");

class Player
{
public:
    void update();
    void render() const;

public:
    Transform Transform;

private:
    void update_Trail();

private:
    float m_inertiaDamping = 0.0f;
    float m_friction = 0.95f;
    vec2f m_ownVelocity = vec2f(3.0f, 18.0f);
    vec2f m_inertia;
    i32 m_currentFrame;

    Pool<Particle, 4> m_trailParticles;
};
