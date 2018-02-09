#pragma once

#include <d3d11.h>
#include "ComPtr.h"
#include "Resource/Texture2D.h"

class D3D11Texture
{
    ComPtr<ID3D11Texture2D> mTexture;
    ComPtr<ID3D11ShaderResourceView> mView;
    ComPtr<ID3D11SamplerState> mSampler;

public:
    D3D11Texture();

    bool IsAvalable() {
        return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
    }
    bool Initialize(ComPtr<ID3D11Device> device, const ResourceHandle<Texture2D>& tex);
    bool Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11Texture2D> tex);

    const ComPtr<ID3D11ShaderResourceView>& GetSubResourceView() const
    {
        return mView;
    }

    const ComPtr<ID3D11SamplerState>& GetSampler() const 
    {
        return mSampler;
    }

    ComPtr<ID3D11ShaderResourceView>& GetSubResourceView()
    {
        return mView;
    }

    ComPtr<ID3D11SamplerState>& GetSampler()
    {
        return mSampler;
    }
};
