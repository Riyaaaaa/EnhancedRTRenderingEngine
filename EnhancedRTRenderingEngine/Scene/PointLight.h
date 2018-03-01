#pragma once

#include "Structure/Structure.h"
#include "Resource/Texture2D.h"

class PointLight
{
public:
    PointLight();
    ~PointLight();

    void SetPoint(const Vector3D& pos) { _point = pos; }
    const Vector3D& GetPoint() { return _point; }

    void SetAttenuation(const Vector3D& att) { _attenuation = att; }
    const Vector3D& GetAttenuation() { return _attenuation; }

    Texture2D GetShadowTexture(int index) { return _cubeDepthTextures[index](); }

    bool Dirty() { return _isDirtyShadow; }
    
protected:
    Vector3D _point;
    Vector3D _attenuation;

    bool _isDirtyShadow;
    ResourceHandle<Texture2D> _cubeDepthTextures[6];
};

