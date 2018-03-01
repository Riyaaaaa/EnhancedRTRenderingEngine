#pragma once

#include "Structure/Structure.h"
#include "Resource/Texture2D.h"
#include "CubeTexture.h"

class PointLight
{
public:
    PointLight();

    void SetPoint(const Vector3D& pos) { _point = pos; }
    const Vector3D& GetPoint() { return _point; }

    void SetAttenuation(const Vector3D& att) { _attenuation = att; }
    const Vector3D& GetAttenuation() { return _attenuation; }

    Texture2D GetShadowTexture(CUBE_DIRECTION dir) { return _shodowTextures[dir](); }

    DirectX::XMMATRIX GetViewMatrix(CUBE_DIRECTION dir);
    DirectX::XMMATRIX GetShadowPerspectiveMatrix(CUBE_DIRECTION dir);

    bool Dirty() { return _isDirtyShadow; }
    bool SetDirty(bool dirty) { return _isDirtyShadow = dirty; }
    
protected:
    Vector3D _point;
    Vector3D _attenuation;

    bool _isDirtyShadow;
    CubeTexture _shodowTextures;
};

