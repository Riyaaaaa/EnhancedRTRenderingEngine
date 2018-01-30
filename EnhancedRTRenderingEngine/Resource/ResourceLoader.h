#pragma once

#include <fstream>
#include "ResourceHandle.h"
#include "Texture2D.h"
#include "PMD/PMDModel.h"

namespace ResourceLoader
{
	ResourceHandle<RowBinary> LoadShader(std::string filename);
	int LoadTexture(std::string filename, ResourceHandle<Texture2D>* outTex);
	ResourceHandle<PMDModel> LoadPMDModel(std::string filename);
	void LoadFBXModel(std::string filename);
};

