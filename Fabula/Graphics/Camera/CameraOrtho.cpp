#include "pch.h"
#include "CameraOrtho.h"

namespace fbl
{

fblBool CameraOrtho::IsValid() const
{
    return super::IsValid() &&
        m_FrameDimension.LeftTop.x < m_FrameDimension.RightBottom.x &&
        m_FrameDimension.LeftTop.y > m_FrameDimension.RightBottom.y;
}

fblMat3x3F CameraOrtho::CalculateProjectionMatrix2D()
{
    fblAssert(IsValid(), "Ortho Camera's parameters are not valid");
    return fblMat3x3F();
}

fblMat4x4F CameraOrtho::CalculateProjectionMatrix3D()
{
    fblAssert(IsValid(), "Ortho Camera's parameters are not valid");
    return fblMat4x4F();
}

void CameraOrtho::SetFrameDimension(const fblRect& dimension)
{
    m_FrameDimension = dimension;
}

const fblRect& CameraOrtho::GetFrameDimension() const
{
    return m_FrameDimension;
}

}
