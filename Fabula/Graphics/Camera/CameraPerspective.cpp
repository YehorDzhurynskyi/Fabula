#include "pch.h"
#include "CameraPerspective.h"

namespace fbl
{

fblBool CameraPerspective::IsValid() const
{
    return super::IsValid() &&
        m_FOV > 0.0f &&
        m_AspectRatio > 0.0f;
}

fblMat3x3F CameraPerspective::CalculateProjectionMatrix2D()
{
    fblAssert(IsValid(), "Perspective Camera's parameters are not valid");
    return fblMat3x3F();
}

fblMat4x4F CameraPerspective::CalculateProjectionMatrix3D()
{
    fblAssert(IsValid(), "Perspective Camera's parameters are not valid");
    return fblMat4x4F();
}

void CameraPerspective::SetAspectRatio(const fblFloat aspectRatio)
{
    m_AspectRatio = aspectRatio;
}

fblFloat CameraPerspective::GetAspectRatio() const
{
    return m_AspectRatio;
}

void CameraPerspective::SetFOV(const fblFloat fov)
{
    m_FOV = fov;
}

fblFloat CameraPerspective::GetFOV() const
{
    return m_FOV;
}

}
