#pragma once

#include <d3d11.h>
#include "RenderingContext.h"

template<class T, class U = void>
T CastToD3D11Formart(U prop);

template<>
DXGI_FORMAT CastToD3D11Formart<DXGI_FORMAT, VertexProperty>(VertexProperty prop) {
	switch (prop)
	{
	case VertexProperty::FloatRG:
		return DXGI_FORMAT_R32G32_FLOAT;
	case VertexProperty::FloatRGB:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case VertexProperty::FloatRGBA:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

template<>
D3D_PRIMITIVE_TOPOLOGY CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY, VertexPrimitiveType>(VertexPrimitiveType prop) {
	switch (prop)
	{
	case VertexPrimitiveType::TRIANGLELIST:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case VertexPrimitiveType::TRIANGLESTRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	default:
		return D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

UINT GetMemoryBlockSize(VertexProperty prop) {
	switch (prop)
	{
	case VertexProperty::FloatRG:
		return 4 * 2;
	case VertexProperty::FloatRGB:
		return 4 * 3;
	case VertexProperty::FloatRGBA:
		return 4 * 4;
		break;
	default:
		return 0;
	}
}
