#include "stdafx.h"
#include "Texture2D.h"

#include "libpng/png.h"
#include "libpng/png.h"
#include "libpng/pngstruct.h"
#include "libpng/pnginfo.h"

Texture2D::Texture2D(png_info_def* info, void* ptr, std::size_t size) : RawBinary(ptr, size) {
    width = info->width;
    height = info->height;
    channels = info->channels;
    stride = info->rowbytes;
}

Texture2D::Texture2D(std::size_t width, std::size_t height, std::size_t channels, void* ptr, std::size_t size) :
    width(width),
    height(height),
    channels(channels),
    stride(width * channels),
    RawBinary(ptr, size) {}