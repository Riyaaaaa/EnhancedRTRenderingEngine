#pragma once

#include <memory>
#include <functional>

#include "RenderScene.h"

class D3D11TextureEffectRenderer
{
public:
    std::shared_ptr<GITexture2D> Apply(GIImmediateCommands* cmd, const GITextureProxy& src, const std::string& effect);
};

