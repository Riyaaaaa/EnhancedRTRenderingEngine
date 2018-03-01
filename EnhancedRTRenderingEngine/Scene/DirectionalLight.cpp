#include "stdafx.h"
#include "DirectionalLight.h"


DirectionalLight::DirectionalLight(const Vector3D& dir) :
    _direction(dir),
    _shadowMap(Texture2D(1980, 1080, 4))
{
}


DirectionalLight::~DirectionalLight()
{
}
