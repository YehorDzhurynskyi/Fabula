#include "pch.h"
#include "Camera.h"

#include "application.h"
#include "SDL_video.h"
#include <algorithm>
#include "Game/Game.h"

const float Camera::g_MinimumVisibleWorldHeight = 14.0f;

vec2f Camera::getScreenSize() const
{
    // TODO cache
    int w, h;
    SDL_GetWindowSize(g_SDLWindow, &w, &h);
    return vec2f(w, h);
}

vec2f Camera::getVisibleWorldBounds() const
{
    // TODO cache
    const vec2f screenSize = getScreenSize();
    const float aspectRatio = screenSize.x / screenSize.y;
    const float hBound = Game::g_MapWidth / aspectRatio;
    const float scale = std::max<float>(g_MinimumVisibleWorldHeight / hBound, 1.0f / Zoom);

    return vec2f(Game::g_MapWidth, hBound) * scale;
}

Transform Camera::toWorldSpace(const Transform& screenSpace) const
{
    return {};
}

Transform Camera::toScreenSpace(const Transform& worldSpace) const
{
    Transform transform;

    const vec2f halfScreenSize = getScreenSize() / 2.0f;

    transform.position = (2.0f * (worldSpace.position - Position)) / getVisibleWorldBounds();
    transform.position *= halfScreenSize;
    transform.position += halfScreenSize;

    transform.size = worldSpace.size / getVisibleWorldBounds();
    transform.size *= getScreenSize();

    return transform;
}

Transform Camera::toNDCSpace(const Transform& worldSpace) const
{
    Transform transform;

    transform.position = (2.0f * (worldSpace.position - Position)) / getVisibleWorldBounds();
    transform.position.y *= -1.0f;
    transform.size = worldSpace.size / getVisibleWorldBounds();

    return transform;
}

