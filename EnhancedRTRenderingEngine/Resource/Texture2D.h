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
    Texture2D(std::size_t width, std::size_t height, std::size_t channels, void* ptr, std::size_t size);
    Texture2D(std::size_t width, std::size_t height, std::size_t channels);

    std::size_t Width() const { return width; }
    std::size_t Height() const { return height; }
    std::size_t Channels() const { return channels; }
    std::size_t Stride() const { return stride; }

    TextureParam GetParam() const;

    void SetTextureName(std::string name) { textureName = name; }

protected:
    std::size_t width, height, channels, stride;
    std::string textureName;
};

