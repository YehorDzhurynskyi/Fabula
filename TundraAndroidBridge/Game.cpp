#include "pch.h"

#include "Game/Game.h"
#include "Game/Level.h"

#include "Renderer.h"
#include "TextRenderer.h"

const float Game::g_MapWidth = 15.0f;
const float Game::g_ChunkGenerationOffset = 0.5f * Camera::g_MinimumVisibleWorldHeight;

Game* g_Game = nullptr;

Game::Game()
{
    m_player.connect(*this);
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
    for (auto& node : m_obstacles)
    {
        if (!node.InUse)
        {
            continue;
        }
        node.Value.update();
    }

    m_obstacles.rescan();

#ifdef _DEBUG
    m_Debug.rescan();
#endif

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.Transform.Position.y + cameraOffset;
}

void Game::render() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, Renderer::get().m_FBO);
    glClear(GL_COLOR_BUFFER_BIT);

    m_player.render();

    for (const auto& node : m_obstacles)
    {
        if (!node.InUse)
        {
            continue;
        }
        node.Value.render();
    }

    {
        Renderer::get().present_Before();

        {
            glBindTexture(GL_TEXTURE_2D, Renderer::get().m_atlas_Texture);
            Renderer::get().m_staticPass.bind();

            glDrawElements(GL_TRIANGLES, Renderer::get().m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

            Renderer::get().m_staticPass.unbind();
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        {
            glBindTexture(GL_TEXTURE_2D, Renderer::get().m_target_Texture);
            Renderer::get().m_motionBlurPass.bind();

            glDrawElements(GL_TRIANGLES, Renderer::get().m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);

            Renderer::get().m_motionBlurPass.unbind();
        }

        Renderer::get().present_After();
    }

    m_player.render_Trail();
    {
        Renderer::get().present_Before();

        glBindTexture(GL_TEXTURE_2D, Renderer::get().m_atlas_Texture);
        Renderer::get().m_staticPass.bind();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, Renderer::get().m_currentSpriteCount);
        Renderer::get().m_staticPass.unbind();

        Renderer::get().present_After();
    }

#ifdef _DEBUG
    for (const auto& node : m_Debug)
    {
        if (!node.InUse)
        {
            continue;
        }
        Renderer::get().render(node.Value.SpriteURI,
                               Camera::get().toNDCSpace(node.Value.Transform),
                               node.Value.ColorTint);
    }
#endif

    for (const auto& node : m_player.BrakeParticles)
    {
        if (!node.InUse)
        {
            continue;
        }

        const Particle& particle = node.Value;
        Renderer::get().render(SpriteURI::Circle,
                               Camera::get().toNDCSpace(particle.Transform),
                               particle.ColorTint);
    }

    for (const auto& node : m_player.TrailParticles)
    {
        if (!node.InUse)
        {
            continue;
        }

        const Particle& particle = node.Value;
        Renderer::get().render(SpriteURI::Circle,
                               Camera::get().toNDCSpace(particle.Transform),
                               particle.ColorTint);
    }

    m_player.render();

    for (const auto& node : m_obstacles)
    {
        if (!node.InUse)
        {
            continue;
        }
        node.Value.render();
    }

    {
        char b[32];
        sprintf(b, "[22, 22, 22, ff]%i m", (i32)m_player.DistanceCovered);
        TextRenderer::get().render_TextCenter(b, vec2f(0.0f, 0.85f), 0.035f);
    }

    {
        char b[32];
        sprintf(b, "[0, 0, 0, ff]%i", (i32)(1.0f / g_DeltaTime));
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.9f), 0.02f);
    }

#ifdef _DEBUG
    {
        char b[32];
        sprintf(b, "[ff, 22, 22, ff]%i", m_player.BrakeParticles.Count);
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.8f), 0.02f);
    }

    {
        char b[32];
        sprintf(b, "[ff, 22, ff, ff]%i", m_player.TrailParticles.Count);
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.7f), 0.02f);
    }

    {
        char b[32];
        sprintf(b, "[00, 63, 4a, ff]%i", m_obstacles.Count);
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.6f), 0.02f);
    }
#endif

    {
        Renderer::get().present_Before();

        glBindTexture(GL_TEXTURE_2D, Renderer::get().m_atlas_Texture);
        Renderer::get().m_staticPass.bind();
        glDrawElements(GL_TRIANGLES, Renderer::get().m_currentSpriteCount * 6, GL_UNSIGNED_SHORT, (void*)0);
        Renderer::get().m_staticPass.unbind();

        Renderer::get().present_After();
    }
}

void Game::generateNextChunk()
{
#if 0
    static bool a;

    a = !a;
    Obstacle& debugPanel = *m_Debug.push();
    debugPanel.Transform.Size = vec2f(Game::g_MapWidth, Camera::g_MinimumVisibleWorldHeight);
    debugPanel.SpriteURI = SpriteURI::Plane;
    debugPanel.ColorTint = a ? FBL_COLOR(0xff, 0x0, 0x0, 0x80) : FBL_COLOR(0x0, 0x0, 0xff, 0x80);

    debugPanel.Transform.Position.y =
        m_player.Transform.Position.y +
        Camera::g_MinimumVisibleWorldHeight +
        g_ChunkGenerationOffset;
#endif

#if 1
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
#else
    generateChunk(Level::L_ChunkSelection[0]);
#endif
}

const Player& Game::getPlayer() const
{
    return m_player;
}

void Obstacle::update()
{
    const ::Transform& ndcTransform = Camera::get().toNDCSpace(Transform);
    m_visible = std::fabsf(ndcTransform.Position.y) < 0.95f;

    if (m_visible)
    {
        m_scale = std::min<float>(1.0f, m_scale + 7.5f * g_DeltaTime);
    }
    else
    {
        m_scale = std::max<float>(0.0f, m_scale - 7.5f * g_DeltaTime);
    }
}

void Obstacle::render() const
{
    ::Transform transform(Transform);

    transform.Size *= m_scale;
    Renderer::get().render(SpriteURI, Camera::get().toNDCSpace(transform), ColorTint);
}

bool Obstacle::isAlive() const
{
    const ::Transform& ndcTransform = Camera::get().toNDCSpace(Transform);
    return ndcTransform.Position.y < 1.1f;
}
