#include "pch.h"

#include "Game/Game.h"
#include "Game/Level.h"

#include "Renderer.h"
#include "Input.h"

const float Game::g_MapWidth = 9.0f;
const float Game::g_ChunkGenerationOffset = 0.5f * Camera::g_MinimumVisibleWorldHeight;

Game::Game()
    : m_atlas("Assets/atlas.png")
{
    m_obstacles.reserve(100);
    m_snowballs.reserve(100);
}

void Game::update()
{
    static float nextYPosToGenerate = g_ChunkGenerationOffset;

    if ((nextYPosToGenerate - g_ChunkGenerationOffset) <= m_player.Transform.Position.y)
    {
        generateNextChunk();
        nextYPosToGenerate += Camera::g_MinimumVisibleWorldHeight;
    }

    m_player.update();

    for (auto& ball : m_snowballs)
    {
        ball.update();
    }

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.Position.y + cameraOffset;
}

void Game::render()
{
    for (const auto& debug : m_Debug)
    {
        Renderer::get().render(debug.SpriteURI,
                               Camera::get().toNDCSpace(debug.Transform),
                               debug.ColorTint);
    }

    static float time;
    time += g_DeltaTime;
    const i32 frame = (i32)(time * 14.0f) % SpriteAtlas::at(AnimatedSpriteURI::Player).NOfFrames;
    Renderer::get().render(AnimatedSpriteURI::Player,
                           frame,
                           Camera::get().toNDCSpace(m_player.Transform),
                           FBL_WHITE_COLOR);

    for (const auto& obstacle : m_obstacles)
    {
        Renderer::get().render(obstacle.SpriteURI,
                               Camera::get().toNDCSpace(obstacle.Transform),
                               obstacle.ColorTint);
    }

    static float xx;
    xx += g_DeltaTime * 10.0f;

    char b[32];

    sprintf_s(b, sizeof(b), "[22, 22, 22, ff]%i m", (i32)xx);

    Renderer::get().renderTextCenter(b, vec2f(0.0f, 0.85f), 0.035f);
}

void Game::generateNextChunk()
{
#if 0
    static bool a;

    a = !a;
    Obstacle debugPanel;
    debugPanel.Transform.Size = vec2f(Game::g_MapWidth, Camera::g_MinimumVisibleWorldHeight);
    debugPanel.SpriteURI = SpriteURI::Plane;
    debugPanel.ColorTint = a ? FBL_COLOR(0xff, 0x0, 0x0, 0x80) : FBL_COLOR(0x0, 0x0, 0xff, 0x80);

    debugPanel.Transform.Position.y =
        m_player.Transform.Position.y +
        Camera::g_MinimumVisibleWorldHeight +
        g_ChunkGenerationOffset;

    m_Debug.push_back(debugPanel);
#endif

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

    Transform.Position.x -= dPos * left;
    Transform.Position.x += dPos * !left;
    Transform.Position.y += 2.0f * dPos;

    const float worldLimit = (Game::g_MapWidth - Transform.Size.x) * 0.5f;
    Transform.Position.x = clamp<float>(Transform.Position.x, -worldLimit, worldLimit);
}

void Snowball::update()
{
    Transform.Position += g_DeltaTime * Velocity;
}
