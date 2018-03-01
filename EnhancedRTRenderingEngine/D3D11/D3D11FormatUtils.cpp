#include "stdafx.h"
#include <d3d11.h>
#include "D3D11FormatUtils.h"

template<>
DXGI_FORMAT CastToD3D11Format<DXGI_FORMAT, VertexProperty>(VertexProperty prop) {
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
D3D_PRIMITIVE_TOPOLOGY CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY, VertexPrimitiveType>(VertexPrimitiveType prop) {
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

template<>
DXGI_SAMPLE_DESC CastToD3D11Format<DXGI_SAMPLE_DESC, MSAAQualityType>(MSAAQualityType prop) {
    switch (prop)
    {
    case MSAAQualityType::RAW_QUALITY:
        return DXGI_SAMPLE_DESC{ 1, 0 };
    default:
        return DXGI_SAMPLE_DESC{ 1, 0 };
    }
}

template<>
DXGI_FORMAT CastToD3D11Format<DXGI_FORMAT, TextureFormat>(TextureFormat prop) {
    switch (prop)
    {
    case TextureFormat::RGBA8_UNORM:
        return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    case TextureFormat::R16_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

template<>
unsigned int CastToD3D11Format<unsigned int, unsigned int>(unsigned int prop) {
    UINT flag = 0;
    if (prop & TextureUsage::SHADER_RESOURCE) {
        flag |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
    }

    if (prop & TextureUsage::STENCIL) {
        flag |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    }

    return flag;
}

DXGI_FORMAT GetShaderResourceFormat(DXGI_FORMAT textureFormat) {
    switch (textureFormat)
    {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
        return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    case DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R16_UNORM;
    default:
        return DXGI_FORMAT_UNKNOWN;
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

