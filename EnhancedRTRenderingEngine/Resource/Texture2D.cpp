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

Texture2D::Texture2D(float width, float height, float channels, void* ptr, std::size_t size) :
    width(width),
    height(height),
    channels(channels),
    RawBinary(ptr, size) {}