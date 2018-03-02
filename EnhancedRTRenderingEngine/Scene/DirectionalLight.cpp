#include "stdafx.h"
#include "DirectionalLight.h"

#include "Common.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(const Vector3D& dir) :
    _direction(dir),
    _shadowMap(Texture2D(1980, 1080, 2))
{
}


DirectionalLight::~DirectionalLight()
{
}

DirectX::XMMATRIX DirectionalLight::GetViewProjection() {
    auto lDir = GetDirection();

    //todo: calculation covering scene AABB
    DirectX::XMVECTOR pos = XMVectorSet(0.0f, 50.0f, -15.0f, 0.0f);
    DirectX::XMVECTOR dir = XMVectorSet(lDir.x, lDir.y, lDir.z, 0.0f);
    DirectX::XMVECTOR hUp = XMVectorSet(0.0f, 0.01f, 1.0f, 0.0f);
    /*static const DirectX::XMMATRIX SHADOW_BIAS = DirectX::XMMATRIX(
    0.5f, 0.0f, 0.0f, 0.0f,
    0.0f, -0.5f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.0f, 1.0f);*/

    return XMMatrixLookToLH(pos, dir, hUp);
}

DirectX::XMMATRIX DirectionalLight::GetPerspectiveProjection() {
    //todo: calculation far plane from position
    return XMMatrixPerspectiveFovLH(D3DXToRadian(45.0f), 1.0f, 1.0f, 100.0f);
}

