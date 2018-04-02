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
public:
    bool IsAvalable() {
        return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
    }
    bool Initialize(GIImmediateCommands* cmd, TextureParam param, const Texture2D& tex = Texture2D{}) override;
    bool Initialize(GIImmediateCommands* cmd, TextureParam param, const std::vector<Texture2D>& textures) override;
    bool Initialize(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> tex, SamplerParam param);
};

typedef std::shared_ptr<D3D11TextureProxyEntity> D3D11TextureProxy;
