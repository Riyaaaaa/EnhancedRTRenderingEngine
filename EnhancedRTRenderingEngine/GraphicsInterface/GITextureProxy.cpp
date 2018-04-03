#include "stdafx.h"
#include "GITextureProxy.h"

#include "GIImmediateCommands.h"

bool GITextureProxyEntity::Initialize(GIImmediateCommands* cmd, TextureParam param, const Texture2D& tex)
{
    std::vector<Texture2D> v;
    if (tex.isValid()) {
        v.push_back(tex);
    }
    return Initialize(cmd, param, v);
}

bool GITextureProxyEntity::Initialize(GIImmediateCommands* cmd, TextureParam param, const std::vector<Texture2D>& textures) {

    mTexture = MakeRef(cmd->CreateTexture2D(param, textures));

    if (!(param.bindFlag & TextureBindTarget::SHADER_RESOURCE)) {
        return true;
    }

    mView = MakeRef(cmd->CreateShaderResourceView(mTexture.get()));
    mSampler = MakeRef(cmd->CreateSamplerState(param.samplerParam));
    return true;
}

bool GITextureProxyEntity::Initialize(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> tex, SamplerParam param) {
    mTexture = tex;
    mView = MakeRef(cmd->CreateShaderResourceView(tex.get()));
    mSampler = MakeRef(cmd->CreateSamplerState(param));

    return true;
}
