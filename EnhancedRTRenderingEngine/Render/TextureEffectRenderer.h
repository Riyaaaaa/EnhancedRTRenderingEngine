#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3D11ConstantBufferBuilder.h"
#include "D3D11SceneInfo.h"

class D3D11TextureEffectRenderer
{
public:
    std::shared_ptr<GITexture2D> Apply(GIImmediateCommands* cmd, const GITextureProxy& src, const std::string& effect);
};

