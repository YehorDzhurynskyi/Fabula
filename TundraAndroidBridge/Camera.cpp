#include "pch.h"
#include "Camera.h"

void Camera::setViewPortSize(vec2i size)
{
    m_viewportSize = size;
}

vec2i Camera::getViewPortSize() const
{
    return m_viewportSize;
}
