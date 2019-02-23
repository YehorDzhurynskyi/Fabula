#include "pch.h"
#include "HUDLayer.h"
#include "Graphics/Renderer.h"
#include "Graphics/Text/TextRenderer.h"
#include "GameLayer.h"

HUDLayer::HUDLayer()
{
    m_clickEventListener.on(EventType::Click, [](const Event& event)
    {
        assert(event.type() == EventType::Click);

        const ClickEvent& clickEvent = AS(const ClickEvent&, event);
        if (clickEvent.NDCXPos > 0.9f &&
            clickEvent.NDCYPos < 0.05f)
        {
            SDL_Log("CLICK");
            return false;
        }

        return true;
    });

    connect(*this);
}

void HUDLayer::update()
{}

void HUDLayer::render() const
{
    {
        Transform t;
        t.Position = vec2f(0.9f, 0.9f);
        t.Size = vec2f(0.05f, 0.05f);

        Renderer::get().render(SpriteURI::IconPause, t, FBL_WHITE_COLOR);
    }

    {
        char b[32];
        sprintf(b, "[22, 22, 22, ff]%i m", (i32)g_Game->m_player.DistanceCovered);
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
        sprintf(b, "[ff, 22, 22, ff]%i", g_Game->m_player.BrakeParticles.Count);
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.8f), 0.02f);
    }

    {
        char b[32];
        sprintf(b, "[ff, 22, ff, ff]%i", g_Game->m_player.TrailParticles.Count);
        TextRenderer::get().render_TextLeft(b, vec2f(-0.8f, 0.7f), 0.02f);
    }

    {
        char b[32];
        sprintf(b, "[00, 63, 4a, ff]%i", g_Game->m_obstacles.Count);
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

void HUDLayer::onConnect(Layer& layer)
{
    m_clickEventListener.bind(layer);
}
