#include "pch.h"
#include "Game/Game.h"
#include "Camera.h"
#include "Input.h"

const float Game::g_MapWidth = 9.0f;

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

        m_obstacles.push_back(obstacle);
        timer = 2.0f;
    }

    m_player.update();

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.position.y + cameraOffset;
}

void Game::render()
{
    m_atlas.draw(SpriteURI::Player, m_player.Transform);
    for (const auto& obstacle : m_obstacles)
    {
        m_atlas.draw(SpriteURI::Rock, obstacle.Transform);
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
