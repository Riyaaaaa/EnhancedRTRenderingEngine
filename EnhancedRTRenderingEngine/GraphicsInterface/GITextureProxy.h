#pragma once
#pragma once

#include <d3d11.h>
#include <memory>
#include "Resource/Texture2D.h"
#include "RenderingContext.h"

class GITextureProxyEntity
{
public:
    virtual bool Initialize(TextureParam param, const Texture2D& tex = Texture2D{}) = 0;
    virtual bool Initialize(TextureParam param, const std::vector<Texture2D>& textures) = 0;
};

typedef std::shared_ptr<GITextureProxyEntity> GITextureProxy;
