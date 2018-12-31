#pragma once

class Player
{
public:
    void update();

public:
    Transform Transform;

private:
    float m_inertiaDamping = 0.0f;
    float m_friction = 0.95f;
    vec2f m_ownVelocity = vec2f(3.0f, 3.0f);
    vec2f m_inertia;
};