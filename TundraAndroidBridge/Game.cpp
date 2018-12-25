#include "pch.h"
#include "Game/Game.h"
#include "Camera.h"
#include "Input.h"

const float Game::g_MapWidth = 10.0f;

Game::Game()
    : m_atlas("Assets/atlas.bmp")
{
    m_obstacles.reserve(100);
    m_snowballs.reserve(100);
}

void Game::update()
{
    static float timer = 0.0f;
    static float ballTimer = 0.0f;

    timer -= g_DeltaTime;
    ballTimer -= g_DeltaTime;
    if (timer <= 0.0f)
    {
        Obstacle obstacle;
        obstacle.Transform.position = m_player.Transform.position;
        obstacle.Transform.position.y += Camera::get().getVisibleWorldBounds().y;
        obstacle.SpriteURI = AS(SpriteURI, rand() % 8);

        obstacle.SpriteURI = rand() % 2 == 0 ? SpriteURI::Tree4 : SpriteURI::Stone;

        switch (obstacle.SpriteURI)
        {
        case SpriteURI::Rock:
        case SpriteURI::Bush:
        case SpriteURI::Stump:
        case SpriteURI::Stone:
        {
            obstacle.Transform.size = vec2f(0.75f, 0.75f);
        } break;
        case SpriteURI::Tree1:
        case SpriteURI::Tree2:
        case SpriteURI::Tree3:
        case SpriteURI::Tree4:
        {
            obstacle.Transform.size = vec2f(1.5f, 1.5f); break;
        } break;
        default:
        {
            assert(!"Unrecognized obstacle");
        } break;
        }

        m_obstacles.push_back(obstacle);
        timer = 2.0f;
    }
    if (ballTimer <= 0.0f)
    {
        Snowball ball;
        ball.Transform.position = m_player.Transform.position;
        ball.Transform.position.y -= Camera::get().getVisibleWorldBounds().y;
        ball.Transform.size = vec2f(2.0f, 2.0f);
        ball.Velocity = 12.0f * (vec2f(0.0f, m_player.Transform.position.y + Camera::get().getVisibleWorldBounds().y) - ball.Transform.position).normalize();

        m_snowballs.push_back(ball);
        ballTimer = 2.0f;
    }

    m_player.update();

    for (auto& ball : m_snowballs)
    {
        ball.update();
    }

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.position.y + cameraOffset;
}

void Game::render()
{
    static float time;
    time += g_DeltaTime;

    m_atlas.draw(AnimatedSpriteURI::Player, m_player.Transform, time * 14.0f);

    for (const auto& ball : m_snowballs)
    {
        m_atlas.draw(AnimatedSpriteURI::Snowball, ball.Transform, time * 30.0f);
    }

    for (const auto& obstacle : m_obstacles)
    {
        m_atlas.draw(obstacle.SpriteURI, obstacle.Transform);
    }
}

void Player::update()
{
    static bool left;
    const float speed = 2.0f;
    const float dPos = speed * g_DeltaTime;

    if (Input::DirectionSwitched)
    {
        left = !left;
    }

    Transform.position.x -= dPos * left;
    Transform.position.x += dPos * !left;
    Transform.position.y += 2.0f * dPos;

    const float worldLimit = (Game::g_MapWidth - Transform.size.x) * 0.5f;
    Transform.position.x = clamp<float>(Transform.position.x, -worldLimit, worldLimit);
}

void Snowball::update()
{
    Transform.position += g_DeltaTime * Velocity;
}
