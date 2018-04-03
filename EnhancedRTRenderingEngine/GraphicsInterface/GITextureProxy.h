#pragma once

#include <memory>
#include "Resource/Texture2D.h"
#include "RenderingContext.h"

#include "GIResource.h"

class GIImmediateCommands;
class GITexture2D;

class GITextureProxyEntity
{
public:
    virtual bool Initialize(GIImmediateCommands* cmd, TextureParam param, const Texture2D& tex = Texture2D{}) = 0;
    virtual bool Initialize(GIImmediateCommands* cmd, TextureParam param, const std::vector<Texture2D>& textures) = 0;
    virtual bool Initialize(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> tex, SamplerParam param = SamplerParam()) = 0;

    std::shared_ptr<GITexture2D> mTexture;
    std::shared_ptr<GIShaderResourceView> mView;
    std::shared_ptr<GISamplerState> mSampler;

public:
    bool IsAvalable() {
        return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
    }

    const std::shared_ptr<GITexture2D>& GetTexture() const
    {
        return mTexture;
    }

    std::shared_ptr<GITexture2D>& GetTexture()
    {
        return mTexture;
    }

    const std::shared_ptr<GIShaderResourceView>& GetSubResourceView() const
    {
        return mView;
    }

    const std::shared_ptr<GISamplerState>& GetSampler() const
    {
        return mSampler;
    }

    std::shared_ptr<GIShaderResourceView>& GetSubResourceView()
    {
        return mView;
    }

    std::shared_ptr<GISamplerState>& GetSampler()
    {
        return mSampler;
    }

};

typedef std::shared_ptr<GITextureProxyEntity> GITextureProxy;

