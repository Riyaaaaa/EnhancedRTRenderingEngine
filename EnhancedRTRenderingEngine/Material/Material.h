#pragma once

#include "Structure/Structure.h"
#include "Resource/ResourceHandle.h"
#include "Resource/Texture2D.h"
#include "MaterialParameters.h"

class Material
{
public:
    Material() {}
    explicit Material(const MaterialParameters& param);
    ~Material();

    Vector3D specular;
    ResourceHandle<> vShader, pShader;
    ResourceHandle<Texture2D> texture;
};

