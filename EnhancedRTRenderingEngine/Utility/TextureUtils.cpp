#include "stdafx.h"
#include "TextureUtils.h"

#include "Algorithms/CompressTexture.h"

#include "FileManager.h"

Texture2D TextureUtils::CreateColorPalletTexture(unsigned int width, unsigned int height) {
    unsigned char* buffer = new unsigned char[width * height * 4 * sizeof(unsigned char)];

    for (unsigned int i = 0; i < height; i++) {
        char g = static_cast<unsigned char>(255 * i / height);
        for (unsigned int j = 0; j < width; j++) {
            *(buffer + i * width * 4 + j * 4) = static_cast<char>(255 * j / width);
            *(buffer + i * width * 4 + j * 4 + 1) = g;
            *(buffer + i * width * 4 + j * 4 + 2) = 0;
            *(buffer + i * width * 4 + j * 4 + 3) = 255;
        }
    }

    return Texture2D(width, height, 4, buffer, sizeof(buffer));
}

Texture2D TextureUtils::CreateUniformTexture(unsigned int width, unsigned int height, Color4B color) {
    unsigned char* buffer = new unsigned char[width * height * 4 * sizeof(unsigned char)];

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            *(buffer + i * width * 4 + j * 4) = color.r;
            *(buffer + i * width * 4 + j * 4 + 1) = color.g;
            *(buffer + i * width * 4 + j * 4 + 2) = color.b;
            *(buffer + i * width * 4 + j * 4 + 3) = color.a;
        }
    }

    return Texture2D(width, height, 4, buffer, sizeof(buffer));
}

std::vector<Texture2D> TextureUtils::CreateMipmaps(Texture2D srcTex, unsigned int miplevels) {
    if (miplevels == 0) {
        miplevels = static_cast<unsigned int>(std::floor(std::log2(srcTex.Width())) + 1);
    }

    std::vector<Texture2D> mipmaps;
    mipmaps.reserve(miplevels);

    mipmaps.push_back(srcTex);

    Texture2D compressed = srcTex;
    Size2Dd size(srcTex.Width(), srcTex.Height());
    for (unsigned int i = 1; i < miplevels; i++) {
        size = size / 2;
        compressed = CompressTexture::NearestNeighbor(compressed, size);
        mipmaps.push_back(compressed);
        FileManager::getInstance()->AddCache<Texture2D>(srcTex.GetTextureName() + std::to_string(i), compressed);
    }

    return mipmaps;
}

std::vector<Texture2D> TextureUtils::CreateMipmaps(TextureCube srcTex, unsigned int miplevels) {
    if (miplevels == 0) {
        miplevels = static_cast<unsigned int>(std::floor(std::log2(srcTex.Size())) + 1);
    }

    std::vector<Texture2D> mipmaps;
    mipmaps.reserve(miplevels * 6);

    for (int j = 0; j < 6; j++) {
        std::vector<Texture2D> compresseds = CreateMipmaps(srcTex.textures[j], miplevels);
        std::copy(compresseds.begin(), compresseds.end(), std::back_inserter(mipmaps));
    }

    return mipmaps;
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