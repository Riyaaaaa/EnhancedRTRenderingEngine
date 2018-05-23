#pragma once

#include "Structure/Structure.h"
#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"

class PointLight
{
public:
    PointLight();

    void SetPoint(const Vector3D& pos);
    const Vector3D& GetPoint() { return _point; }

    void SetAttenuationRadius(float att_rad) { 
        _invSqrRadius = 1.0f / std::max(att_rad * att_rad, 0.00001f);
    }
    float InvSquareRadius() const { return _invSqrRadius; }

    float Intensity() const {
        return _intensity;
    }

    ResourceHandle<Texture2D> GetShadowTexture(CUBE_DIRECTION dir) { return _shodowTextures[dir]; }
    Size2D GetShadowResolution() { return _shadowResolution; }

    DirectX::XMMATRIX* GetViewMatrixes();
    DirectX::XMMATRIX GetViewMatrix(CUBE_DIRECTION dir);
    DirectX::XMMATRIX GetShadowPerspectiveMatrix();

    bool Dirty() { return _isDirtyShadow; }
    bool SetDirty(bool dirty) { return _isDirtyShadow = dirty; }
    
protected:
    Vector3D _point;
    float _invSqrRadius;
    float _intensity;

    bool _isDirtyMatrix;
    bool _isDirtyShadow;
    DirectX::XMMATRIX _viewMatrixes[6];
    TextureCube _shodowTextures;
    Size2D _shadowResolution;
};

