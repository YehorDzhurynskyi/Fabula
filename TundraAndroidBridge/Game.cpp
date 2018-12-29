#include "pch.h"

#include "Game/Game.h"
#include "Game/Level.h"
#include "Game/FontRenderer.h"

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
    static float time;
    time += g_DeltaTime;

    for (const auto& debug : m_Debug)
    {
        Renderer::get().render(debug.SpriteURI, debug.Transform, debug.ColorTint);
    }

    const i32 frame = (i32)(time * 14.0f) % SpriteAtlas::at(AnimatedSpriteURI::Player).NOfFrames;
    Renderer::get().render(AnimatedSpriteURI::Player, frame, m_player.Transform, FBL_WHITE_COLOR);

    for (const auto& obstacle : m_obstacles)
    {
        Renderer::get().render(obstacle.SpriteURI, obstacle.Transform, obstacle.ColorTint);
    }

    //const vec2f ppos = vec2f(0.0f, -0.95f) * Camera::get().getScreenSize() * 0.5f + Camera::get().getScreenSize() * 0.5f;

    //FontRenderer::get().renderTextCenter("1024m", ppos, 0.025f);

    //const vec2f pos = vec2f(0.0f, -0.85f) * Camera::get().getScreenSize() * 0.5f + Camera::get().getScreenSize() * 0.5f;
    //FontRenderer::get().renderTextCenter("1m", pos, 0.035f);
}

void Game::generateNextChunk()
{
#if 0
    static bool a;

    a = !a;
    Obstacle debugPanel;
    debugPanel.Transform.size = vec2f(Game::g_MapWidth, Camera::g_MinimumVisibleWorldHeight);
    debugPanel.SpriteURI = a ? SpriteURI::Debug1 : SpriteURI::Debug2;

    debugPanel.Transform.position.y =
        m_player.Transform.position.y +
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
