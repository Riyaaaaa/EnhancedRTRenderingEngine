#include "stdafx.h"
#include "D3D11TextureProxy.h"
#include "D3D11FormatUtils.h"

#include "Constant/RenderConfig.h"
#include "RenderingContext.h"

#include "Utility/TextureUtils.h"

#include "Common.h"

bool D3D11TextureProxyEntity::Initialize(GIImmediateCommands* cmd, TextureParam param, const Texture2D& tex)
{
    std::vector<Texture2D> v;
    if (tex.isValid()) {
        v.push_back(tex);
    }
    return Initialize(cmd, param, v);
}

bool D3D11TextureProxyEntity::Initialize(GIImmediateCommands* cmd, TextureParam param, const std::vector<Texture2D>& textures) {
    
    cmd->CreateTexture2D(param, textures);

    if (!(param.bindFlag & TextureBindTarget::SHADER_RESOURCE)) {
        return true;
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(CastToD3D11Format<DXGI_FORMAT>(param.format));

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

    mView = MakeRef(cmd->CreateShaderResourceView(mTexture.get()));

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.samplerParam.addressMode);
    samplerDesc.AddressV = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.samplerParam.addressMode);
    samplerDesc.AddressW = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.samplerParam.addressMode);
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    mSampler = MakeRef(cmd->CreateSamplerState(param.samplerParam));

    return true;
}

bool D3D11TextureProxyEntity::Initialize(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> tex, SamplerParam param) {
    mTexture = tex;

    auto texparam = mTexture->GetTextureParam();

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = GetShaderResourceFormat(CastToD3D11Format<DXGI_FORMAT>(texparam.format));

    if (texparam.type == TextureType::TextureCube) {
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
        SRVDesc.TextureCube.MostDetailedMip = 0;
        SRVDesc.TextureCube.MipLevels = texparam.mipLevels;
    }
    else {
        if (texparam.isMultiSampling) {
            if (texparam.arraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
                SRVDesc.Texture2DArray.MipLevels = texparam.mipLevels;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
                SRVDesc.Texture2DArray.MipLevels = texparam.mipLevels;
                SRVDesc.Texture2DArray.ArraySize = texparam.arraySize;
            }
        }
        else {
            if (texparam.arraySize == 1) {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
            }
            else {
                SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
            }
        }
    }

    mView = MakeRef(cmd->CreateShaderResourceView(tex.get()));

    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.AddressV = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.AddressW = CastToD3D11Format<D3D11_TEXTURE_ADDRESS_MODE>(param.addressMode);
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    mSampler = MakeRef(cmd->CreateSamplerState(param));

    return true;
}
