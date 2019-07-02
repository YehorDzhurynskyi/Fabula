#pragma once

namespace fbl
{

class Camera
{
public:
    Camera();
    Camera(const Camera& rhs) = default;
    Camera& operator=(const Camera& rhs) = default;
    Camera(Camera&& rhs) = default;
    Camera& operator=(Camera&& rhs) = default;
    virtual ~Camera() = default;

    virtual fblBool IsValid() const;

    fblMat3x3F CalculateViewMatrix2D();
    fblMat4x4F CalculateViewMatrix3D();
    virtual fblMat3x3F CalculateProjectionMatrix2D() = 0;
    virtual fblMat4x4F CalculateProjectionMatrix3D() = 0;

    void LookAt(const fblV3F& position, const fblV3F& poi, const fblV3F& up);

    void SetFar(const fblFloat far);
    void SetNear(const fblFloat near);
    fblFloat GetFar() const;
    fblFloat GetNear() const;

protected:
    fblV3F m_Position;
    fblV3F m_LookAtDirection;
    fblV3F m_UpDirection;

    fblFloat m_Far;
    fblFloat m_Near;
};

}
