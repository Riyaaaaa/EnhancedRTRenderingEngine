#pragma once

#include "ResourceHandle.h"

#include "RenderingContext.h"

struct png_info_def;

class Texture2D : public RawBinary
{
    using RawBinary::RawBinary;
public:
    Texture2D() {}
    Texture2D(png_info_def* info, void* ptr, std::size_t size);
    Texture2D(unsigned int width, unsigned int height, unsigned int channels, const void* ptr, std::size_t size);
    Texture2D(unsigned int width, unsigned int height, unsigned int channels);

    unsigned int Width() const { return width; }
    unsigned int Height() const { return height; }
    unsigned int Channels() const { return channels; }
    unsigned int Stride() const { return stride; }

    TextureParam GetParam() const;

    void SetTextureName(std::string name) { textureName = name; }
    std::string GetTextureName() { return textureName; }
protected:
    unsigned int width, height, channels, stride;
    std::string textureName;
};

