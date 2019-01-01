#pragma once

class Player
{
public:
    void update();
    void render() const;

public:
    Transform Transform;

private:
    float m_inertiaDamping = 0.0f;
    float m_friction = 0.95f;
    vec2f m_ownVelocity = vec2f(3.0f, 9.0f);
    vec2f m_inertia;
    i32 m_currentFrame;
};