#pragma once

#include "Resource/ResourceHandle.h"
#include "Resource/Texture2D.h"

#include "MaterialParameters.h"

class Material
{
public:
    Material() {}
    explicit Material(const MaterialParameters& param);
    ~Material();

    ResourceHandle<> vShader, pShader;
    ResourceHandle<Texture2D> texture;
};

