#pragma once

#include "Layer/Event/Event.h"
#include "Layer/Node.h"
#include "Library/Singleton.h"

class Camera final : public Node, public Singleton<Camera>
{
friend class Singleton<Camera>;
private:
    Camera();

public:
    static const float g_MinimumVisibleWorldHeight;

    vec2f getScreenSize() const;
    vec2f getVisibleWorldBounds() const;

    Transform toWorldSpace(const Transform& screenSpace) const;
    Transform toScreenSpace(const Transform& worldSpace) const;
    Transform toNDCSpace(const Transform& worldSpace) const;
    vec2f toNDCSpace(const vec2f& worldSpace) const;

protected:
    void onConnect(Layer& layer) override;
    void onWindowSizeChanged(i32 width, i32 height);

public:
    vec2f Position;
    float Zoom = 1.0f;

private:
    EventListener m_windowResizedListener;

    vec2f m_screenSize;
    vec2f m_visibleBounds;
};
