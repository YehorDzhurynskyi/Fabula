#pragma once

#include "Camera.h"

namespace fbl
{

class CameraOrtho : public Camera
{
public:
    using super = Camera;

public:
    fblBool IsValid() const override;

    fblMat3x3F CalculateProjectionMatrix2D() const override;
    fblMat4x4F CalculateProjectionMatrix3D() const override;

    void SetFrameDimension(const fblRect& dimension);
    const fblRect& GetFrameDimension() const;

protected:
    fblRect m_FrameDimension;
};

}
