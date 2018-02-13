#include "stdafx.h"
#include "D3D11DepthStencilTarget.h"
#include "Common.h"


D3D11DepthStencilTarget::D3D11DepthStencilTarget():
    _depthTexture(nullptr),
    _depthStencilView(nullptr)
{
}

bool D3D11DepthStencilTarget::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> hpDeviceContext) {
    D3D11_TEXTURE2D_DESC hTexture2dDesc;
    //todo: use window mgr
    hTexture2dDesc.Width = 1980;
    hTexture2dDesc.Height = 1080;
    hTexture2dDesc.MipLevels = 1;
    hTexture2dDesc.ArraySize = 1;
    hTexture2dDesc.Format = DXGI_FORMAT_R16_TYPELESS;
    hTexture2dDesc.SampleDesc = DXGI_SAMPLE_DESC{ 1, 0 };
    hTexture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    hTexture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    hTexture2dDesc.CPUAccessFlags = 0;
    hTexture2dDesc.MiscFlags = 0;
    if (FAILED(device->CreateTexture2D(&hTexture2dDesc, NULL, _depthTexture.ToCreator()))) {
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = DXGI_FORMAT_D16_UNORM;
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;
    if (FAILED(device->CreateDepthStencilView(_depthTexture.Get(), &hDepthStencilViewDesc, _depthStencilView.ToCreator()))) {
        return false;
    }

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    if (FAILED(device->CreateSamplerState(&samplerDesc, _sampler.ToCreator())))
    {
        return false;
    }

    _texture.Initialize(device, _depthTexture);

    return true;
}
