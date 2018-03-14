#include "stdafx.h"
#include "DirectionalLight.h"

#include "Common.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(const Vector3D& dir) :
    _direction(dir),
    _shadowResolution({ 1980, 1000 })
{
}


DirectionalLight::~DirectionalLight()
{
}

DirectX::XMMATRIX DirectionalLight::GetViewProjection() {
    auto lDir = GetDirection();

    //todo: calculation covering scene AABB
    DirectX::XMVECTOR pos = XMVectorSet(0.0f, 40.0f, -15.0f, 1.0f);
    DirectX::XMVECTOR dir = XMVectorSet(lDir.x, lDir.y, lDir.z, 1.0f);
    DirectX::XMVECTOR hUp = XMVectorSet(0.0f, 0.1f, 1.0f, 1.0f);
    /*static const DirectX::XMMATRIX SHADOW_BIAS = DirectX::XMMATRIX(
    0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f);*/

    return XMMatrixLookToLH(pos, dir, hUp);
}

DirectX::XMMATRIX DirectionalLight::GetPerspectiveProjection() {
    //todo: calculation far plane from position
    return XMMatrixPerspectiveFovLH(D3DXToRadian(90.0f), 1980 / 1000, 1.0f, 100.0f);
}

