#pragma once

#include "MeshObject.h"
#include "Structure/Structure.h"

class SkyBox : public MeshObject<MainVertex>
{
public:
    SkyBox(const std::string& skyboxTextureDirectory);

    const TextureCube& GetCubeTextureResource();
};

