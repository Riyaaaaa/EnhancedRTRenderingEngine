#pragma once

#include <fstream>
#include "ResourceHandle.h"

namespace ResourceLoader
{
	ResourceHandle LoadShader(std::string filename);
	void LoadFBXModel(std::string filename);
};

