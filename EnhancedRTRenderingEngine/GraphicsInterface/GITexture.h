#pragma once
#pragma once

#include <d3d11.h>

#include "Resource/Texture2D.h"
#include "RenderingContext.h"

class GITexture
{
public:
    virtual bool Initialize(TextureParam param, const Texture2D& tex = Texture2D{}) = 0;
    virtual bool Initialize(TextureParam param, const std::vector<Texture2D>& textures) = 0;
};
