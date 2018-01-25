#pragma once

#include <fstream>
#include "ResourceHandle.h"
#include "Texture2D.h"

namespace ResourceLoader
{
	ResourceHandle LoadShader(std::string filename);
	int LoadTexture(std::string filename, Texture2D* outTex);
	void LoadFBXModel(std::string filename);
};

