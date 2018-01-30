#pragma once

#include "ResourceHandle.h"

struct png_info_def;

// support png only
class Texture2D : public RowBinary
{
	using RowBinary::RowBinary;
public:
	Texture2D() {}
	Texture2D(png_info_def* info, void* ptr, SIZE_T size);

	SIZE_T Width() const { return width; }
	SIZE_T Height() const { return height; }
	SIZE_T Channels() const { return stride; }
	SIZE_T Stride() const { return channels; }

protected:
	SIZE_T width, height, channels, stride;
};

