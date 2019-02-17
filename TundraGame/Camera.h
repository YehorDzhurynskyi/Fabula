#pragma once

#include "Event/Event.h"
#include "Node.h"
#include "Singleton.h"

class Camera final : public Node, public Singleton<Camera>
{
public:
    using super = Node;

    static const float g_MinimumVisibleWorldHeight;

    Camera();

public:
    vec2f getScreenSize() const;
    vec2f getVisibleWorldBounds() const;

    Transform toWorldSpace(const Transform& screenSpace) const;
    Transform toScreenSpace(const Transform& worldSpace) const;
    Transform toNDCSpace(const Transform& worldSpace) const;
    vec2f toNDCSpace(const vec2f& worldSpace) const;

private:
    void onWindowSizeChanged(i32 width, i32 height);

public:
    vec2f Position;
    float Zoom = 1.0f;

private:
    EventListener m_windowResizedListener;

    vec2f m_screenSize;
    vec2f m_visibleBounds;
};

