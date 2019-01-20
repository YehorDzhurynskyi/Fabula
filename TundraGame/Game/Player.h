#pragma once

#include <vector>

struct Particle
{
    vec2f Position;
    vec2f Velocity;
    u32 Color;
    float Life;
};

static_assert(sizeof(Particle) == 24, "size should be 24");

class Player
{
public:
    Player();

    void update();
    void render() const;

public:
    Transform Transform;

private:
    void update_Trail();

private:
    float m_inertiaDamping;
    float m_friction;
    vec2f m_ownVelocity;
    vec2f m_inertia;
    i32 m_currentFrame;

    std::vector<Particle> m_trailParticles;
};
