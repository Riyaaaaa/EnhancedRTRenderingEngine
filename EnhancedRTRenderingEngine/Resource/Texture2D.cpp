#include "stdafx.h"
#include "Texture2D.h"

#include "libpng/png.h"
#include "libpng/png.h"
#include "libpng/pngstruct.h"
#include "libpng/pnginfo.h"

Texture2D::Texture2D(png_info_def* info, void* ptr, SIZE_T size) : ResourceHandle::ResourceHandle(ptr, size) {
	width = info->width;
	height = info->height;
	channels = info->channels;
	stride = info->rowbytes;
}