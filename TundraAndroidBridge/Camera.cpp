#include "pch.h"
#include "Camera.h"

#include "application.h"
#include "SDL_video.h"
#include <algorithm>
#include "Game/Game.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

const float Camera::g_MinimumVisibleWorldHeight = 24.0f;

Camera::Camera()
    : m_windowResizedListener(this, EventType::WindowResized, [this](const Event& event)
{
    assert(event.type() == EventType::WindowResized);
    const WindowResizedEvent& windowResizedEvent = AS(const WindowResizedEvent&, event);
    onWindowSizeChanged(windowResizedEvent.Width, windowResizedEvent.Height);
})
{
    i32 w;
    i32 h;
    SDL_GetWindowSize(g_SDLWindow, &w, &h);
    onWindowSizeChanged(w, h);
}

void Camera::onWindowSizeChanged(i32 width, i32 height)
{
    glViewport(0, 0, width, height);

    m_screenSize = vec2f(width, height);

    const float aspectRatio = m_screenSize.x / m_screenSize.y;
    const float hBound = Game::g_MapWidth / aspectRatio;
    m_visibleBounds = vec2f(Game::g_MapWidth, hBound);
}

vec2f Camera::getScreenSize() const
{
    return m_screenSize;
}

vec2f Camera::getVisibleWorldBounds() const
{
    const float scale = std::max<float>(g_MinimumVisibleWorldHeight / m_visibleBounds.y, 1.0f / Zoom);
    return m_visibleBounds * scale;
}

Transform Camera::toWorldSpace(const Transform& screenSpace) const
{
    return {};
}

Transform Camera::toScreenSpace(const Transform& worldSpace) const
{
    Transform transform;

    const vec2f halfScreenSize = getScreenSize() / 2.0f;

    transform.Position = (2.0f * (worldSpace.Position - Position)) / getVisibleWorldBounds();
    transform.Position *= halfScreenSize;
    transform.Position += halfScreenSize;

    transform.Size = worldSpace.Size / getVisibleWorldBounds();
    transform.Size *= getScreenSize();

    return transform;
}

Transform Camera::toNDCSpace(const Transform& worldSpace) const
{
    Transform transform;

    transform.Position = (2.0f * (worldSpace.Position - Position)) / getVisibleWorldBounds();
    transform.Position.y *= -1.0f;
    transform.Size = worldSpace.Size / getVisibleWorldBounds();

    return transform;
}

