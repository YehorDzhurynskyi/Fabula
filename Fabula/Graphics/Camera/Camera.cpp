#include "Fabula/pch.h"
#include "Camera.h"

namespace fbl
{

Camera::Camera()
    : m_Position()
    , m_LookAtDirection()
    , m_UpDirection()
    , m_Far(0.0f)
    , m_Near(0.0f)
{}

fblBool Camera::IsValid() const
{
    return m_Far > m_Near && m_Near > 0.0f;
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
    
}

void Camera::SetFar(const fblFloat far)
{
    m_Far = far;
}

void Camera::SetNear(const fblFloat near)
{
    m_Near = near;
}

fblFloat Camera::GetFar() const
{
    return m_Far;
}

fblFloat Camera::GetNear() const
{
    return m_Near;
}

}
