#pragma once

#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"

namespace TextureUtils
{
    Texture2D CreateColorPalletTexture(unsigned int width, unsigned int height);
    Texture2D CreateUniformTexture(unsigned int width, unsigned int height, Color4B color);

    std::vector<Texture2D> CreateMipmaps(Texture2D srcTex, unsigned int miplevels);
    std::vector<Texture2D> CreateMipmaps(TextureCube srcTex, unsigned int miplevels);

    TextureFormat GetShaderResourceFormat(TextureFormat textureFormat);
    TextureFormat ZBufferFormatToTextureFormat(TextureFormat textureFormat);
};

