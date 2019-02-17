#pragma once

#include "Pool.h"
#include "Event/Event.h"
#include "RotatingBuffer.h"
#include "Node.h"

struct Particle
{
    Transform Transform;
    vec2f Velocity;
    u32 ColorTint;
    float Life;

    bool isAlive() const
    {
        return Life > 0.0f;
    }
};

static_assert(sizeof(Particle) == 32, "unexpected size");

class Player : public Node
{
public:
    Player();

    void update();
    void render() const;
    void render_Trail() const;

private:
    void update_Trail();
    void update_Brake();

public:
    Transform Transform;
    Pool<Particle, 50> TrailParticles;
    Pool<Particle, 50> BrakeParticles;
    float DistanceCovered = 0.0f;

private:
    float m_inertiaDamping = 0.0f;
    float m_friction = 0.96f;
    vec2f m_velocity;
    vec2f m_ownVelocity = vec2f(3.0f, 3.0f);
    vec2f m_inertia;
    i32 m_currentFrame;
    EventListener m_directionSwitchListener;
    RotatingBuffer<vec2f, 16> m_playerTrailBuffer;
};
