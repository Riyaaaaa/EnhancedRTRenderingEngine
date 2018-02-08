#pragma once

#include <DirectXMath.h>

namespace TransformUtils {
	DirectX::XMVECTOR GetProjectedPosition(const DirectX::XMVECTOR& pos, const DirectX::XMMATRIX& World, const DirectX::XMMATRIX& View, const DirectX::XMMATRIX& Perspective);
}

