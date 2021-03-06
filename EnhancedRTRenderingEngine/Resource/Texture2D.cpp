
#include "Texture2D.h"

#include <libpng/png.h>
#include <libpng/png.h>
#include <libpng/pngstruct.h>
#include <libpng/pnginfo.h>

Texture2D::Texture2D(png_info_def* info, void* ptr, std::size_t size) : RawBinary(ptr, size) {
    width = info->width;
    height = info->height;
    channels = info->channels;
    stride = static_cast<unsigned int>(info->rowbytes);
}

Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned int channels, const void* ptr, std::size_t size) :
    width(width),
    height(height),
    channels(channels),
    stride(width * channels),
    RawBinary(ptr, size) {}

Texture2D::Texture2D(unsigned int width, unsigned int height, unsigned int channels) : width(width),
height(height),
channels(channels),
stride(width * channels) {
    _ptr = new unsigned char[height * width * channels];
    _size = height * width * channels;
}

TextureParam Texture2D::GetParam() const {
    TextureParam param;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.format = TextureFormat::RGBA8_UNORM;
    param.height = height;
    param.width = width;
    param.type = TextureType::Texture2D;
    param.arraySize = 1;

    return param;
}