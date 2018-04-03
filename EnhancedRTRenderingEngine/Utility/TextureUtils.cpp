#include "stdafx.h"
#include "TextureUtils.h"

Texture2D TextureUtils::CreateColorPalletTexture(std::size_t width, std::size_t height) {
    unsigned char* buffer = new unsigned char[width * height * 4 * sizeof(unsigned char)];

    for (std::size_t i = 0; i < height; i++) {
        char g = static_cast<unsigned char>(255 * i / height);
        for (std::size_t j = 0; j < width; j++) {
            *(buffer + i * width * 4 + j * 4) = static_cast<char>(255 * j / width);
            *(buffer + i * width * 4 + j * 4 + 1) = g;
            *(buffer + i * width * 4 + j * 4 + 2) = 0;
            *(buffer + i * width * 4 + j * 4 + 3) = 255;
        }
    }

    return Texture2D(width, height, 4, buffer, sizeof(buffer));
}

Texture2D TextureUtils::CreateUniformTexture(std::size_t width, std::size_t height, Color4B color) {
    unsigned char* buffer = new unsigned char[width * height * 4 * sizeof(unsigned char)];

    for (std::size_t i = 0; i < height; i++) {
        for (std::size_t j = 0; j < width; j++) {
            *(buffer + i * width * 4 + j * 4) = color.r;
            *(buffer + i * width * 4 + j * 4 + 1) = color.g;
            *(buffer + i * width * 4 + j * 4 + 2) = color.b;
            *(buffer + i * width * 4 + j * 4 + 3) = color.a;
        }
    }

    return Texture2D(width, height, 4, buffer, sizeof(buffer));
}

TextureFormat TextureUtils::GetShaderResourceFormat(TextureFormat textureFormat) {
    switch (textureFormat)
    {
    case TextureFormat::R16_TYPELESS:
        return TextureFormat::R16_UNORM;
    case TextureFormat::R32_TYPELESS:
        return TextureFormat::R32_UINT;
    default:
        return textureFormat;
    }
}

TextureFormat TextureUtils::ZBufferFormatToTextureFormat(TextureFormat textureFormat) {
    switch (textureFormat)
    {
    case TextureFormat::D16_UNORM:
        return TextureFormat::R16_TYPELESS;
    case TextureFormat::D24_UNORM_S8_UINT:
        return TextureFormat::R24G8_TYPELESS;
    default:
        return textureFormat;
    }
}