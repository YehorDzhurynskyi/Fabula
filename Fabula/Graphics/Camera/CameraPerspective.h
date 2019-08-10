#pragma once

#include "Camera.h"

namespace fbl
{

class CameraPerspective : public Camera
{
public:
    using super = Camera;

public:
    fblBool IsValid() const override;

    fblMat3x3F CalculateProjectionMatrix2D() const override;
    fblMat4x4F CalculateProjectionMatrix3D() const override;

    void SetFOV(fblFloat fov);
    fblFloat GetFOV() const;

protected:
    fblFloat m_FOV = 90.0f;
};

}
