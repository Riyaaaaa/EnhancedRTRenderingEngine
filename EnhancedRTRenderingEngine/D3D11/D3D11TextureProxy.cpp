#include "stdafx.h"
#include "D3D11TextureProxy.h"
#include "D3D11FormatUtils.h"

#include "Constant/RenderConfig.h"
#include "RenderingContext.h"

#include "Common.h"

D3D11TextureProxy::D3D11TextureProxy(const ComPtr<ID3D11Device>& device) :
    mDevice(device),
    mTexture(nullptr),
    mView(nullptr),
    mSampler(nullptr){
}


bool D3D11TextureProxy::Initialize(TextureParam param, const Texture2D& tex)
{
    std::vector<Texture2D> v;
    if (tex.isValid()) {
        v.push_back(tex);
    }
    return Initialize(param, v);
}

bool D3D11TextureProxy::Initialize(TextureParam param, const std::vector<Texture2D>& textures) {
    std::vector<D3D11_SUBRESOURCE_DATA> initData;
    D3D11_SUBRESOURCE_DATA* initDataPtr = nullptr;
    if (!textures.empty()) {
        param.width = textures[0].Width();
        param.height = textures[0].Height();
        param.arraySize = textures.size();

        initData.resize(param.arraySize);
        for (int i = 0; i < param.arraySize; i++) {
            initData[i].pSysMem = textures[i].get();
            initData[i].SysMemPitch = textures[i].Stride();
        }

        initDataPtr = &initData[0];
    }
    
    D3D11_TEXTURE2D_DESC desc;
    desc.Width = param.width;
    desc.Height = param.height;
    desc.MipLevels = 1;
    desc.ArraySize = param.arraySize;
    desc.Format = CastToD3D11Format<DXGI_FORMAT>(param.format);
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = CastToD3D11Format<D3D11_USAGE>(param.usage);
    desc.BindFlags = CastToD3D11Format<UINT>(param.bindFlag);
    desc.CPUAccessFlags = CastToD3D11Format<UINT>(param.accessFlag);
    desc.MiscFlags = 0;

    switch (param.type) {
    case TextureType::TextureCube:
        desc.MiscFlags |= D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_TEXTURECUBE;
        break;
    }

    auto hr = mDevice->CreateTexture2D(&desc, initDataPtr, mTexture.ToCreator());
    if (FAILED(hr)) {
        return false;
    }

    if (!(desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)) {
        return true;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(desc.Format);

    switch (param.type) {
    case TextureType::Texture2D:
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        SRVDesc.Texture2D.MipLevels = 1;
        break;
    case TextureType::Texture2DArray:
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        SRVDesc.Texture2DArray.MipLevels = 1;
        SRVDesc.Texture2DArray.ArraySize = textures.size();
        break;
    case TextureType::TextureCube:
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SRVDesc.TextureCube.MostDetailedMip = 0;
        SRVDesc.TextureCube.MipLevels = 1;
        break;
    }

    hr = mDevice->CreateShaderResourceView(mTexture.Get(), &SRVDesc, mView.ToCreator());
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
    hr = mDevice->CreateSamplerState(&samplerDesc, mSampler.ToCreator());
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}

bool D3D11TextureProxy::Initialize(const ComPtr<ID3D11Texture2D>& tex) {
    mTexture = tex;

    D3D11_TEXTURE2D_DESC texDesc;
    mTexture->GetDesc(&texDesc);

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(texDesc.Format);

    if (texDesc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SRVDesc.TextureCube.MostDetailedMip = 0;
        SRVDesc.TextureCube.MipLevels = texDesc.MipLevels;
    }
    else {
        if (texDesc.SampleDesc.Count == 1) {
            if (texDesc.ArraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                SRVDesc.Texture2DArray.MipLevels = texDesc.MipLevels;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                SRVDesc.Texture2DArray.MipLevels = texDesc.MipLevels;
                SRVDesc.Texture2DArray.ArraySize = texDesc.ArraySize;
            }
        }
        else {
            if (texDesc.ArraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            }
        }
    }

    auto hr = mDevice->CreateShaderResourceView(tex.Get(), &SRVDesc, mView.ToCreator());
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
    hr = mDevice->CreateSamplerState(&samplerDesc, mSampler.ToCreator());
    if (FAILED(hr))
    {
        return false;
    }

    return true;
}
