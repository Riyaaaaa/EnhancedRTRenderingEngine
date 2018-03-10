#pragma once

#include <d3d11.h>
#include "ComPtr.h"
#include "Resource/Texture2D.h"
#include "RenderingContext.h"

class D3D11Texture
{
    ComPtr<ID3D11Device> mDevice;
    ComPtr<ID3D11Texture2D> mTexture;
    ComPtr<ID3D11ShaderResourceView> mView;
    ComPtr<ID3D11SamplerState> mSampler;

public:
    D3D11Texture(){}
    D3D11Texture(const ComPtr<ID3D11Device>& device);

    bool IsAvalable() {
        return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
    }
    bool Initialize(TextureParam param, const Texture2D& tex = Texture2D{});
    bool Initialize(TextureParam param, const std::vector<Texture2D>& textures);
    bool Initialize(ComPtr<ID3D11Texture2D> tex);

    const ComPtr<ID3D11Texture2D>& GetTexture() const
    {
        return mTexture;
    }

    ComPtr<ID3D11Texture2D>& GetTexture()
    {
        return mTexture;
    }

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
