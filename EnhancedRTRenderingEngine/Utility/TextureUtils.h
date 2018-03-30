#pragma once

#include "Resource/Texture2D.h"

namespace TextureUtils
{
    Texture2D CreateColorPalletTexture(std::size_t width, std::size_t height);
    Texture2D CreateUniformTexture(std::size_t width, std::size_t height, Color4B color);

    TextureFormat GetShaderResourceFormat(TextureFormat textureFormat);
};

