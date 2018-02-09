#include "stdafx.h"
#include "TransformUtils.h"

using namespace DirectX;

DirectX::XMVECTOR TransformUtils::GetProjectedPosition(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& World, const DirectX::XMMATRIX& View, const DirectX::XMMATRIX& Perspective) {
    auto projected = DirectX::XMVector4Transform(
        DirectX::XMVector4Transform(
            DirectX::XMVector4Transform(pos, World),
            View),
        Perspective);
    return (projected / XMVectorGetByIndex(projected, 3));
}