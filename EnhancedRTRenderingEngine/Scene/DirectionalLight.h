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

    const ResourceHandle<Texture2D>& GetShadowTexture() { return _shadowMap; }
    Size GetShadowResolution() { return _shadowResolution; }

    DirectX::XMMATRIX GetViewProjection();
    DirectX::XMMATRIX GetPerspectiveProjection();

    Vector3D _direction;

protected:
    ResourceHandle<Texture2D> _shadowMap;
    Size _shadowResolution;
};

