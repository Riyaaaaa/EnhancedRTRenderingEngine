#include "stdafx.h"
#include <d3d11.h>
#include "D3D11FormatUtils.h"

template<>
DXGI_FORMAT CastToD3D11Format<DXGI_FORMAT, VertexProperty>(VertexProperty prop) {
    switch (prop)
    {
    case VertexProperty::FloatR:
        return DXGI_FORMAT_R32_FLOAT;
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
    case VertexPrimitiveType::LINELIST:
        return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST;
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
    case TextureFormat::RGBA8_UNORM_SRGB:
        return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    case TextureFormat::RGBA16_UNORM:
        return DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM;
    case TextureFormat::R16_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS;
    case TextureFormat::R32_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
    case TextureFormat::R24G8_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS;
    case TextureFormat::D16_UNORM:
        return DXGI_FORMAT::DXGI_FORMAT_D16_UNORM;
    case TextureFormat::D24_UNORM_S8_UINT:
        return DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
    default:
        return DXGI_FORMAT_UNKNOWN;
    }
}

template<>
D3D11_USAGE CastToD3D11Format<D3D11_USAGE, TextureUsage>(TextureUsage prop) {
    D3D11_USAGE usage;

    switch (prop) {
    case TextureUsage::Default:
        return D3D11_USAGE::D3D11_USAGE_DEFAULT;
    case TextureUsage::Immutable:
        return D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
    case TextureUsage::Dynamic:
        return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
    case TextureUsage::Staging:
        return D3D11_USAGE::D3D11_USAGE_STAGING;
    }

    return D3D11_USAGE::D3D11_USAGE_DEFAULT;
}

template<>
unsigned int CastToD3D11Format<unsigned int, ResourceAccessFlag>(ResourceAccessFlag prop) {
    UINT flag = 0;

    switch (prop) {
    case ResourceAccessFlag::R:
        flag |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ;
        break;
    case ResourceAccessFlag::W:
        flag |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        break;
    case ResourceAccessFlag::RW:
        flag |= D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
        break;
    }

    return flag;
}

template<>
unsigned int CastToD3D11Format<unsigned int, unsigned int>(unsigned int prop) {
    UINT flag = 0;
    if (prop & TextureBindTarget::SHADER_RESOURCE) {
        flag |= D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
    }

    if (prop & TextureBindTarget::STENCIL) {
        flag |= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
    }

    if (prop & TextureBindTarget::RENDER_TARGET) {
        flag |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;
    }

    return flag;
}

template<>
D3D11_TEXTURE_ADDRESS_MODE CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE, TextureAddressMode>(TextureAddressMode mode) {
    switch (mode) {
    case TextureAddressMode::WRAP:
        return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
    case TextureAddressMode::MIRROR:
        return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR;
    case TextureAddressMode::CLAMP:
        return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP;
    case TextureAddressMode::BORADER:
        return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER;
    }

    return D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
}

template<>
VertexProperty CastToGIFormat<VertexProperty, DXGI_FORMAT>(DXGI_FORMAT prop) {
    switch (prop)
    {
    case DXGI_FORMAT_R32G32_FLOAT:
        return VertexProperty::FloatRG;
    case DXGI_FORMAT_R32G32B32_FLOAT:
        return VertexProperty::FloatRGB;
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
        return VertexProperty::FloatRGBA;
    default:
        return VertexProperty::NONE;
    }
}

template<>
VertexPrimitiveType CastToGIFormat<VertexPrimitiveType, D3D_PRIMITIVE_TOPOLOGY>(D3D_PRIMITIVE_TOPOLOGY prop) {
    switch (prop)
    {
    case D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
        return VertexPrimitiveType::TRIANGLELIST;
    case D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
        return VertexPrimitiveType::TRIANGLESTRIP;
    case D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST:
        return VertexPrimitiveType::LINELIST;
    default:
        return VertexPrimitiveType::NONE;
    }
}

template<>
MSAAQualityType CastToGIFormat<MSAAQualityType, DXGI_SAMPLE_DESC>(DXGI_SAMPLE_DESC prop) {

    if (prop.Quality == 1) {
        return MSAAQualityType::RAW_QUALITY;
    }
    
    return MSAAQualityType::RAW_QUALITY;
}

template<>
TextureFormat CastToGIFormat<TextureFormat, DXGI_FORMAT>(DXGI_FORMAT prop) {
    switch (prop)
    {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
        return TextureFormat::RGBA8_UNORM;
    case DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_UNORM:
        return TextureFormat::RGBA16_UNORM;
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
        return TextureFormat::RGBA8_UNORM_SRGB;
    case DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS:
        return TextureFormat::R16_TYPELESS;
    case DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS:
        return TextureFormat::R32_TYPELESS;
    case DXGI_FORMAT::DXGI_FORMAT_R24G8_TYPELESS:
        return TextureFormat::R24G8_TYPELESS;
    default:
        return TextureFormat::R16_UNORM;
    }
}

template<>
TextureUsage CastToGIFormat<TextureUsage, D3D11_USAGE>(D3D11_USAGE prop) {
    switch (prop) {
    case D3D11_USAGE::D3D11_USAGE_DEFAULT:
        return TextureUsage::Default;
    case D3D11_USAGE::D3D11_USAGE_IMMUTABLE:
        return TextureUsage::Immutable;
    case D3D11_USAGE::D3D11_USAGE_DYNAMIC:
        return TextureUsage::Dynamic;
    case D3D11_USAGE::D3D11_USAGE_STAGING:
        return TextureUsage::Staging;
    }

    return TextureUsage::Default;
}

template<>
ResourceAccessFlag CastToGIFormat<ResourceAccessFlag, unsigned int>(unsigned int prop) {
    UINT flag = 0;

    if (flag & D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ & D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE) {
        return ResourceAccessFlag::RW;
    }
    else if (flag & D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_READ) {
        return ResourceAccessFlag::R;
    }
    else if (flag & D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE) {
        return ResourceAccessFlag::W;
    }

    return ResourceAccessFlag::None;
}

template<>
unsigned int CastToGIFormat<unsigned int, unsigned int>(unsigned int prop) {
    UINT flag = 0;
    if (prop & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE) {
        flag |= TextureBindTarget::SHADER_RESOURCE;
    }

    if (prop & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL) {
        flag |= TextureBindTarget::STENCIL;
    }

    if (prop & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET) {
        flag |= TextureBindTarget::RENDER_TARGET;
    }

    return flag;
}

template<>
TextureAddressMode CastToGIFormat<TextureAddressMode, D3D11_TEXTURE_ADDRESS_MODE>(D3D11_TEXTURE_ADDRESS_MODE mode) {
    switch (mode) {
    case D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP:
        return TextureAddressMode::WRAP;
    case D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_MIRROR:
        return TextureAddressMode::MIRROR;
    case D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_CLAMP:
        return TextureAddressMode::CLAMP;
    case D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_BORDER:
        return TextureAddressMode::BORADER;
    }

    return TextureAddressMode::WRAP;
}

DXGI_FORMAT GetShaderResourceFormat(DXGI_FORMAT textureFormat) {
    switch (textureFormat)
    {
    case DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM:
        return DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
    case DXGI_FORMAT::DXGI_FORMAT_R16_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R16_UNORM;
    case DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS:
        return DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
    default:
        return textureFormat;
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

