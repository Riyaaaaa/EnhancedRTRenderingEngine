#pragma once

#include <fstream>
#include "ResourceHandle.h"
#include "Texture2D.h"

#include "PMD/PMDModel.h"
#include "X/DXModel.h"

namespace ResourceLoader
{
    ResourceHandle<RawBinary> LoadShader(std::string filename);
    int LoadTexture(std::string filename, ResourceHandle<Texture2D>* outTex);
    int LoadPNG(std::string filename, ResourceHandle<Texture2D>* outTex);
    int LoadBMP(const std::string& filename, ResourceHandle<Texture2D>* outTex);
    
    void LoadFBXModel(std::string filename);
    ResourceHandle<DXModel> LoadDXModel(std::string filename);
    ResourceHandle<PMDModel> LoadPMDModel(std::string filename);
};

