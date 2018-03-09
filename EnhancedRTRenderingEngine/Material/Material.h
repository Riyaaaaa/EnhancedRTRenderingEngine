#pragma once

#include "Structure/Structure.h"
#include "Resource/ResourceHandle.h"
#include "Resource/Texture2D.h"
#include "MaterialParameters.h"

class Material
{
public:
    Material() {}
    ~Material();

    explicit Material(const MaterialParameters& param);
    Material(const MaterialParameters& param, const Texture2D& texture);

    float roughness, metallic;
    Vector3D specular;
    TextureType type = TextureType::Texture2D;
    RawBinary vShader, pShader;
    Texture2D texture;
    std::vector<Texture2D> cubeTexture;
};

