#pragma once

#include "Structure/Structure.h"
#include "Resource/ResourceHandle.h"
#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"
#include "MaterialParameters.h"
#include "Constant/GraphicConstants.h"

class Material
{
public:
    Material() {}
    ~Material();

    explicit Material(const MaterialParameters& param);
    Material(const MaterialParameters& param, const Texture2D& texture);

    float roughness, metallic;
    Vector3D specular;
    ShadingType shadingType = ShadingType::BasePass;
    TextureType type = TextureType::None;
    RawBinary vShader, pShader;
    Texture2D texture;
    TextureCube cubeTexture;

    static Material Default;
};

