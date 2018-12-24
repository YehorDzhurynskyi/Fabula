#include "pch.h"
#include "Camera.h"

#include "application.h"
#include "SDL_video.h"

const float Camera::VisibleWorldHeight = 10.0f;

vec2f Camera::getScreenSize() const
{
    // TODO cache size value
    int w, h;
    SDL_GetWindowSize(g_SDLWindow, &w, &h);
    return vec2f(w, h);
}

vec2f Camera::getVisibleWorldBounds() const
{
    const vec2f screenSize = getScreenSize();
    const float aspectRatio = screenSize.x / screenSize.y;

    return vec2f(VisibleWorldHeight * aspectRatio, VisibleWorldHeight);
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

