#pragma once

#include "Structure/Structure.h"
#include "Resource/Texture2D.h"

class DirectionalLight
{
public:
    DirectionalLight(const Vector3D& dir);
    ~DirectionalLight();

    void SetDirection(const Vector3D& dir) { _direction = dir; }
    const Vector3D& GetDirection() { return _direction; }

    Size GetShadowResolution() { return _shadowResolution; }

    DirectX::XMMATRIX GetViewProjection();
    DirectX::XMMATRIX GetPerspectiveProjection();

    Vector3D _direction;

protected:
    Size _shadowResolution;
};

