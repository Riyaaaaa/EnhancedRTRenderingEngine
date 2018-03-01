#include "stdafx.h"
#include "D3D11Texture.h"
#include "D3D11FormatUtils.h"

#include "Constant/RenderConfig.h"
#include "RenderingContext.h"

#include "Common.h"

D3D11Texture::D3D11Texture() :
    mTexture(nullptr),
    mView(nullptr),
    mSampler(nullptr){
}


bool D3D11Texture::Initialize(ComPtr<ID3D11Device> device, const Texture2D& tex, TextureParam param)
{
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = tex.Width();
    desc.Height = tex.Height();
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = CastToD3D11Format<DXGI_FORMAT>(param.format);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = CastToD3D11Format<UINT>(param.usage);
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem = tex.get();
    initData.SysMemPitch = tex.Stride();
    //initData.SysMemSlicePitch = tex.Size();

    auto hr = device->CreateTexture2D(&desc, &initData, mTexture.ToCreator());
    if (FAILED(hr)) {
        return false;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(desc.Format);
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    hr = device->CreateShaderResourceView(mTexture.Get(), &SRVDesc, mView.ToCreator());
    if (FAILED(hr))
    {
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

    // Create the texture sampler state.
    hr = device->CreateSamplerState(&samplerDesc, mSampler.ToCreator());
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool D3D11Texture::Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11Texture2D> tex) {
    mTexture = tex;

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R16_UNORM;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;

    auto hr = device->CreateShaderResourceView(tex.Get(), &SRVDesc, mView.ToCreator());
    if (FAILED(hr))
    {
        return false;
    }
    
    mTexture = tex;

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

    // Create the texture sampler state.
    hr = device->CreateSamplerState(&samplerDesc, mSampler.ToCreator());
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
