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

    void SetPosition(const fblV3F& position);
    const fblV3F& GetPosition() const;

    void SetZoom(fblFloat zoom);
    fblFloat GetZoom() const;
    void SetFar(fblFloat far);
    fblFloat GetFar() const;
    void SetNear(fblFloat near);
    fblFloat GetNear() const;
    void SetAspectRatio(fblFloat aspectRatio);
    fblFloat GetAspectRatio() const;

protected:
    fblV3F m_Position;
    fblV3F m_LookAtDirection;
    fblV3F m_UpDirection;

    fblFloat m_Zoom;
    fblFloat m_Far;
    fblFloat m_Near;
    fblFloat m_AspectRatio;
};

}
