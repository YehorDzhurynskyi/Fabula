#include "Fabula/pch.h"
#include "CameraOrtho.h"
#include "glm/ext/matrix_clip_space.inl"

namespace fbl
{

fblBool CameraOrtho::IsValid() const
{
    return super::IsValid() &&
        m_FrameDimension.LeftTop.x < m_FrameDimension.RightBottom.x &&
        m_FrameDimension.LeftTop.y > m_FrameDimension.RightBottom.y;
}

fblMat3x3F CameraOrtho::CalculateProjectionMatrix2D() const
{
    fblAssert(IsValid(), "Ortho Camera's parameters are not valid");
    return fblMat3x3F();
}

fblMat4x4F CameraOrtho::CalculateProjectionMatrix3D() const
{
    fblAssert(IsValid(), "Ortho Camera's parameters are not valid");

    fblMat4x4F mat = glm::ortho(m_FrameDimension.LeftTop.x * m_Zoom,
                                m_FrameDimension.RightBottom.x * m_Zoom,
                                m_FrameDimension.RightBottom.y * m_Zoom,
                                m_FrameDimension.LeftTop.y * m_Zoom,
                                m_Near,
                                m_Far);

    mat[0][0] /= GetAspectRatio();
    return mat;
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
