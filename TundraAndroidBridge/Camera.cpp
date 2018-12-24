#include "pch.h"
#include "Camera.h"

#include "application.h"
#include "SDL_video.h"
#include <algorithm>

const float Camera::DesiredVisibleWorldWidth = 8.0f;
const float Camera::MinimumWorldHeight = 10.0f;

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
    const float hBound = DesiredVisibleWorldWidth / aspectRatio;
    const float scale = std::max<float>(MinimumWorldHeight / hBound, 1.0f / m_zoom);

    return vec2f(DesiredVisibleWorldWidth, hBound) * scale;
}

Transform Camera::toWorldSpace(const Transform& screenSpace) const
{
    return {};
}

Transform Camera::toScreenSpace(const Transform& worldSpace) const
{
    Transform transform;

    const vec2f halfScreenSize = getScreenSize() / 2.0f;

    transform.position = (2.0f * worldSpace.position) / getVisibleWorldBounds();
    transform.position *= halfScreenSize;
    transform.position += halfScreenSize;

    transform.size = worldSpace.size / getVisibleWorldBounds();
    transform.size *= getScreenSize();

    return transform;
}

