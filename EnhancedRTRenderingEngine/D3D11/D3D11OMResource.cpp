#include "stdafx.h"
#include "D3D11OMResource.h"
#include "RenderingContext.h"
#include "Common.h"


D3D11OMResource::D3D11OMResource(const ComPtr<ID3D11Device>& device, Size size) :
    _depthStencilView(nullptr),
    _device(device),
    _RTVTexture(device),
    _DSVTexture(device),
    _size(size)
{
}

bool D3D11OMResource::InitializeRenderTarget(ComPtr<ID3D11DeviceContext> hpDeviceContext, bool useAsShaderResource) {
    
    TextureParam param;
    param.format = TextureFormat::RGBA16_UNORM;
    param.bindFlag = TextureBindTarget::RENDER_TARGET | (useAsShaderResource ? TextureBindTarget::SHADER_RESOURCE : 0);
    param.width = _size.w;
    param.height = _size.h;
    param.arraySize = 1;
    param.samplerParam.addressMode = TextureAddressMode::CLAMP;

    _RTVTexture.Initialize(param);

    if (FAILED(_device->CreateRenderTargetView(_RTVTexture.GetTexture().Get(), nullptr, _renderTarget.ToCreator()))) {
        return false;
    }

    return true;
}

bool D3D11OMResource::InitializeDepthStencilView(ComPtr<ID3D11DeviceContext> hpDeviceContext, bool useAsShaderResource) {

    TextureParam param;
    param.format = TextureFormat::R16_TYPELESS;
    param.bindFlag = TextureBindTarget::STENCIL | (useAsShaderResource ? TextureBindTarget::SHADER_RESOURCE : 0);
    param.width = _size.w;
    param.height = _size.h;
    param.arraySize = 1;
    param.samplerParam.addressMode = TextureAddressMode::CLAMP;

    _DSVTexture.Initialize(param);

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D16_UNORM;
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;
    if (FAILED(_device->CreateDepthStencilView(_DSVTexture.GetTexture().Get(), &hDepthStencilViewDesc, _depthStencilView.ToCreator()))) {
        return false;
    }

    return true;
}

