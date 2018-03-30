#pragma once

#include <d3d11.h>
#include <memory>
#include "ComPtr.h"
#include "D3D11Resources.h"
#include "GraphicsInterface/GITextureProxy.h"
#include "GraphicsInterface/GIResourceattribute.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "Resource/Texture2D.h"
#include "RenderingContext.h"

class D3D11TextureProxyEntity : public GITextureProxyEntity, public ResourceAttribute::Creatable<D3D11TextureProxyEntity>
{
    std::shared_ptr<D3D11Texture2D> mTexture;
    std::shared_ptr<D3D11ShaderResourceView> mView;
    std::shared_ptr<D3D11SamplerState> mSampler;

    TextureParam _param;

public:
    bool IsAvalable() {
        return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
    }
    bool Initialize(GIImmediateCommands* cmd, TextureParam param, const Texture2D& tex = Texture2D{});
    bool Initialize(GIImmediateCommands* cmd, TextureParam param, const std::vector<Texture2D>& textures);
    bool Initialize(GIImmediateCommands* cmd, const ComPtr<ID3D11Texture2D>& tex, SamplerParam param = SamplerParam());

    const TextureParam& GetParam() const {
        return _param;
    }

    const std::shared_ptr<D3D11Texture2D>& GetTexture() const
    {
        return mTexture;
    }

    std::shared_ptr<D3D11Texture2D>& GetTexture()
    {
        return mTexture;
    }

    const std::shared_ptr<D3D11ShaderResourceView>& GetSubResourceView() const
    {
        return mView;
    }

    const std::shared_ptr<D3D11SamplerState>& GetSampler() const
    {
        return mSampler;
    }

    std::shared_ptr<D3D11ShaderResourceView>& GetSubResourceView()
    {
        return mView;
    }

    std::shared_ptr<D3D11SamplerState>& GetSampler()
    {
        return mSampler;
    }
};

typedef std::shared_ptr<D3D11TextureProxyEntity> D3D11TextureProxy;
