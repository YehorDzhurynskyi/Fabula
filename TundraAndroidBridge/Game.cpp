#include "pch.h"

#include "Game/Game.h"
#include "Game/Level.h"

#include "Renderer.h"
#include "Input.h"

const float Game::g_MapWidth = 15.0f;
const float Game::g_ChunkGenerationOffset = 0.5f * Camera::g_MinimumVisibleWorldHeight;

Game* g_Game = nullptr;

Game::Game()
    : m_atlas("Assets/atlas.png")
{
    assert(g_Game == nullptr);
    g_Game = this;

    m_obstacles.reserve(100);
}

Game::~Game()
{
    assert(g_Game != nullptr);
    g_Game = nullptr;
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
    for (auto& obstacle : m_obstacles)
    {
        obstacle.update();
    }

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.Position.y + cameraOffset;
}

void Game::render()
{
    Transform t;
    t.Position = Camera::get().Position;
    t.Size = vec2f(Game::g_MapWidth, Camera::get().getVisibleWorldBounds().y);

    Renderer::get().render(SpriteURI::Plane,
                           Camera::get().toNDCSpace(t),
                           FBL_COLOR(0xf8, 0xf8, 0xf8, 0xff));

    for (const auto& debug : m_Debug)
    {
        Renderer::get().render(debug.SpriteURI,
                               Camera::get().toNDCSpace(debug.Transform),
                               debug.ColorTint);
    }

    m_player.render();

    for (const auto& obstacle : m_obstacles)
    {
        obstacle.render();
    }

    Renderer::get().present_MotionBlured();

    for (auto& node : m_player.TrailParticles)
    {
        if (!node.InUse)
        {
            continue;
        }

        Particle& particle = node.Value;
        Renderer::get().render(SpriteURI::Circle,
                               Camera::get().toNDCSpace(particle.Transform),
                               particle.ColorTint);
    }

    m_player.render();

    for (const auto& obstacle : m_obstacles)
    {
        obstacle.render();
    }

    static float xx;
    xx += g_DeltaTime * 10.0f;

    char b[32];
    sprintf(b, "[22, 22, 22, ff]%i m", (i32)xx);
    Renderer::get().render_TextCenter(b, vec2f(0.0f, 0.85f), 0.035f);

    Renderer::get().present_Static();
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

void Obstacle::update()
{
    const ::Transform& ndcTransform = Camera::get().toNDCSpace(Transform);
    m_visible = std::fabsf(ndcTransform.Position.y) < 1.0f;

    if (m_visible)
    {
        m_scale = std::min<float>(1.0f, m_scale + 5.0f * g_DeltaTime);
    }
}

void Obstacle::render() const
{
    ::Transform transform(Transform);

    transform.Size *= m_scale;
    Renderer::get().render(SpriteURI, Camera::get().toNDCSpace(transform), ColorTint);
}

const Player& Game::getPlayer() const
{
    return m_player;
}
