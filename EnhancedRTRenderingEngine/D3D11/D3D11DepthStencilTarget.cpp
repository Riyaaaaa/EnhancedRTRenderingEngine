#include "stdafx.h"
#include "D3D11DepthStencilTarget.h"
#include "RenderingContext.h"
#include "Common.h"


D3D11DepthStencilTarget::D3D11DepthStencilTarget() :
    _depthStencilView(nullptr)
{
}

bool D3D11DepthStencilTarget::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> hpDeviceContext, const Texture2D& tex) {
    
    TextureParam param;
    param.format = TextureFormat::R16_TYPELESS;
    param.usage = TextureUsage::SHADER_RESOURCE | TextureUsage::STENCIL;

    _texture.Initialize(device, tex, param);

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = DXGI_FORMAT_D16_UNORM;
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;
    if (FAILED(device->CreateDepthStencilView(_texture.GetTexture().Get(), &hDepthStencilViewDesc, _depthStencilView.ToCreator()))) {
        return false;
    }

    return true;
}

