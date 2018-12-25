#include "pch.h"
#include "Game/Game.h"
#include "Camera.h"
#include "Input.h"

const float Game::g_MapWidth = 8.0f;

Game::Game()
    : m_atlas("Assets/atlas.bmp")
    , m_obstacles(100)
{}

void Game::update()
{
    static float timer = 0.0f;

    timer -= g_DeltaTime;
    if (timer <= 0.0f)
    {
        Obstacle obstacle;
        obstacle.Transform.position = m_player.Transform.position;
        obstacle.Transform.position.y += Camera::get().getVisibleWorldBounds().y;
        obstacle.SpriteURI = AS(SpriteURI, rand() % 6);

        switch (obstacle.SpriteURI)
        {
        case SpriteURI::Rock:
        case SpriteURI::Bush:
        case SpriteURI::Stump:
        {
            obstacle.Transform.size = vec2f(0.75f, 0.75f);
        } break;
        case SpriteURI::Tree1:
        case SpriteURI::Tree2:
        case SpriteURI::Tree3:
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

    m_player.update();

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.position.y + cameraOffset;
}

void Game::render()
{
    static float time;

    time += g_DeltaTime;

    m_atlas.draw(AnimatedSpriteURI::Player, m_player.Transform, time * 14.0f);
    for (const auto& obstacle : m_obstacles)
    {
        m_atlas.draw(obstacle.SpriteURI, obstacle.Transform);
    }
}

void Player::update()
{
    const float speed = 5.0f;
    const float dPos = speed * g_DeltaTime;

    Transform.position.x -= dPos * Input::MovedLeft;
    Transform.position.x += dPos * Input::MovedRight;
    Transform.position.y += dPos;

    const float worldLimit = (Game::g_MapWidth - Transform.size.x) * 0.5f;
    Transform.position.x = clamp<float>(Transform.position.x, -worldLimit, worldLimit);
}
