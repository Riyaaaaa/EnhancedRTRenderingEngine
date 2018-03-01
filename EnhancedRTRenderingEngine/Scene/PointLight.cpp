#include "stdafx.h"
#include "PointLight.h"

#include "Common.h"

using namespace DirectX;

PointLight::PointLight() :
    _isDirtyShadow(true)
{
}

DirectX::XMMATRIX PointLight::GetViewMatrix(CUBE_DIRECTION dir) {
    DirectX::XMVECTOR pos = XMVectorSet(_point.x, _point.y, _point.z, 0.0f);

    return XMMatrixLookToLH(pos, CubeTexture::lookAt[dir], CubeTexture::up[dir]);
}

DirectX::XMMATRIX PointLight::GetShadowPerspectiveMatrix(CUBE_DIRECTION dir) {
    //todo far plane calculate from attenuation
    return XMMatrixPerspectiveFovLH(D3DXToRadian(90.0f), 1.0f, 0.1f, 10.0f);
}
