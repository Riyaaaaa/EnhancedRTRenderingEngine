#include "stdafx.h"
#include "PointLight.h"

#include "Common/Common.h"

using namespace DirectX;

PointLight::PointLight() :
    _intensity(50000),
    _isDirtyShadow(true),
    _isDirtyMatrix(true),
    _shadowResolution({1000, 1000})
{
}

void PointLight::SetPoint(const Vector3D& pos) {
    _point = pos; 
    _isDirtyMatrix = true;
}

DirectX::XMMATRIX PointLight::GetViewMatrix(CUBE_DIRECTION dir) {
    return GetViewMatrixes()[dir];
}

DirectX::XMMATRIX* PointLight::GetViewMatrixes() {
    if (_isDirtyMatrix) {
        for (int i = 0; i < 6; i++) {
            DirectX::XMVECTOR pos = XMVectorSet(_point.x, _point.y, _point.z, 0.0f);
            auto dir = static_cast<CUBE_DIRECTION>(i);
            _viewMatrixes[i] = XMMatrixLookToLH(pos, TextureCube::lookAt[dir], TextureCube::up[dir]);
        }
        _isDirtyMatrix = false;
    }
    return _viewMatrixes;
}

DirectX::XMMATRIX PointLight::GetShadowPerspectiveMatrix() {
    //todo far plane calculate from attenuation
    return XMMatrixPerspectiveFovLH(D3DXToRadian(90.0f), 1.0f, 0.10f, 100.0f);
}
