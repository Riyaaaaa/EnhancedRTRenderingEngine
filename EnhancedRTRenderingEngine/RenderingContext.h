#pragma once

#include <string>
#include <d3d11.h>

#include "Constant/RenderConfig.h"

struct RenderingContext
{
	std::string VSName, PSName;
	VertexProperty vProperty;
	VertexPrimitiveType pTyppe;
};

