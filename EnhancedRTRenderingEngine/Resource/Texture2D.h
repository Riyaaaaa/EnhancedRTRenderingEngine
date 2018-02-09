#pragma once

#include "ResourceHandle.h"

struct png_info_def;

// support png only
class Texture2D : public RawBinary
{
    using RawBinary::RawBinary;
public:
    Texture2D() {}
    Texture2D(png_info_def* info, void* ptr, std::size_t size);
    Texture2D(float width, float height, float channels, void* ptr, std::size_t size);

    std::size_t Width() const { return width; }
    std::size_t Height() const { return height; }
    std::size_t Channels() const { return stride; }
    std::size_t Stride() const { return channels; }

protected:
    std::size_t width, height, channels, stride;
};

