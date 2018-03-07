#include "stdafx.h"
#include "D3D11DepthStencilTarget.h"
#include "RenderingContext.h"
#include "Common.h"


D3D11DepthStencilTarget::D3D11DepthStencilTarget() :
    _depthStencilView(nullptr)
{
}

bool D3D11DepthStencilTarget::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> hpDeviceContext, Size size) {
    
    TextureParam param;
    param.format = TextureFormat::R16_TYPELESS;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE | TextureBindTarget::STENCIL;
    param.width = size.w;
    param.height = size.h;
    param.arraySize = 1;

    _texture.Initialize(device, param);

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = DXGI_FORMAT_D16_UNORM;
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;
    if (FAILED(device->CreateDepthStencilView(_texture.GetTexture().Get(), &hDepthStencilViewDesc, _depthStencilView.ToCreator()))) {
        return false;
    }

    return true;
}

