#pragma once

#include <string>
#include <d3d11.h>

#include <vector>

#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

struct VertexLayout {
	const char* name;
	VertexProperty vProperty;
};

struct RenderingContext
{
	std::string VSName, PSName;
	std::vector<VertexLayout> layouts;

	VertexPrimitiveType pType;
};

template<class VertType>
std::vector<VertexLayout> CreateVertexLayout();


