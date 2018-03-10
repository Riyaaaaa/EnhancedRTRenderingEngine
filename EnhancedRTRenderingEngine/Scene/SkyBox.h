#pragma once

#include "MeshObject.h"
#include "Structure/Structure.h"

class SkyBox : public MeshObject<PMDVertex>
{
public:
    SkyBox(const std::string& skyboxTextureDirectory);

    const std::vector<Texture2D>& GetCubeTextureResource();
};

