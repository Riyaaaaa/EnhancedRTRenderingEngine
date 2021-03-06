
#include "Material.h"

#include "Resource/ResourceLoader.h"

Material::Material(const MaterialParameters& param)
{
    if (param.textureName != "") {
        ResourceLoader::LoadTexture(param.textureName, texture);
        type = TextureType::Texture2D;
    }
    vShader = ResourceLoader::LoadShader(param.vsName);
    pShader = ResourceLoader::LoadShader(param.psName);
    baseColor = param.baseColor;
    roughness = param.roughness;
    metallic = param.metallic;
}

Material::Material(const MaterialParameters& param, const Texture2D& tex) :
    texture({ tex })
{
    type = TextureType::Texture2D;
    vShader = ResourceLoader::LoadShader(param.vsName);
    pShader = ResourceLoader::LoadShader(param.psName);
    baseColor = param.baseColor;
    roughness = param.roughness;
    metallic = param.metallic;
}

Material::~Material()
{
}

Material Material::Default = Material(MaterialParameters{ "LightingVertexShader", "LightingPSBaseColor", "", Vector3D(0.8f, 0.8f, 0.8f), 0.0f, 0.5f });
