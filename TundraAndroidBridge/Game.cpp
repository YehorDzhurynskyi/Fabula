#include "pch.h"

#include "Game/Game.h"
#include "Game/Level.h"

#include "Camera.h"
#include "Input.h"

const float Game::g_MapWidth = 9.0f;

Game::Game()
    : m_atlas("Assets/atlas.png")
{
    m_obstacles.reserve(100);
    m_snowballs.reserve(100);
}

void Game::update()
{
    static float time;

    time -= g_DeltaTime;
    if (time <= 0.0f)
    {
        generateNextChunk();
        time = 3.0f;
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

void Game::generateNextChunk()
{
    switch (rand() % 3)
    {
    case 0:
    {
        generateChunk(Level::S_ChunkSelection[0]);
    } break;
    case 1:
    {
        generateChunk(Level::M_ChunkSelection[0]);
    } break;
    case 2:
    {
        generateChunk(Level::L_ChunkSelection[0]);
    } break;
    default:
    {
        assert(!"Shouldn't be here");
    } break;
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
