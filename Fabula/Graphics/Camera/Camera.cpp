#include "Fabula/pch.h"
#include "Camera.h"

namespace fbl
{

Camera::Camera()
    : m_Position()
    , m_LookAtDirection()
    , m_UpDirection()
    , m_Zoom(1.0f)
    , m_Far(0.0f)
    , m_Near(0.0f)
    , m_AspectRatio(1.0f)
{}

fblBool Camera::IsValid() const
{
    return m_Far > m_Near && m_Near > 0.0f && m_Zoom > 0.0f;
}

fblMat3x3F Camera::CalculateViewMatrix2D()
{
    fblAssert(IsValid(), "Camera's parameters are not valid");
    return fblMat3x3F();
}

fblMat4x4F Camera::CalculateViewMatrix3D()
{
    fblAssert(IsValid(), "Camera's parameters are not valid");
    return fblMat4x4F();
}

void Camera::LookAt(const fblV3F& position, const fblV3F& poi, const fblV3F& up)
{
    // TODO: implement
}

void Camera::SetPosition(const fblV3F& position)
{
    m_Position = position;
}

const fblV3F& Camera::GetPosition() const
{
    return m_Position;
}

void Camera::SetZoom(fblFloat zoom)
{
    assert(zoom > 0.0f);
    m_Zoom = zoom;
}

fblFloat Camera::GetZoom() const
{
    return m_Zoom;
}

void Camera::SetFar(fblFloat far)
{
    assert(far > 0.0f);
    m_Far = far;
}

fblFloat Camera::GetFar() const
{
    return m_Far;
}

void Camera::SetNear(fblFloat near)
{
    assert(near > 0.0f);
    m_Near = near;
}

fblFloat Camera::GetNear() const
{
    return m_Near;
}

void Camera::SetAspectRatio(fblFloat aspectRatio)
{
    assert(aspectRatio > 0.0f);
    m_AspectRatio = aspectRatio;
}

fblFloat Camera::GetAspectRatio() const
{
    return m_AspectRatio;
}

}
