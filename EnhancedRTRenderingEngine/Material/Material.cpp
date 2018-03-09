#include "stdafx.h"
#include "Material.h"

#include "Resource/ResourceLoader.h"

Material::Material(const MaterialParameters& param)
{
    if (param.textureName != "") {
        ResourceLoader::LoadTexture(param.textureName, texture);
    }
    vShader = ResourceLoader::LoadShader(param.vsName);
    pShader = ResourceLoader::LoadShader(param.psName);
    roughness = param.roughness;
    metallic = param.metallic;
}

Material::Material(const MaterialParameters& param, const Texture2D& tex) :
    texture({ tex })
{
    vShader = ResourceLoader::LoadShader(param.vsName);
    pShader = ResourceLoader::LoadShader(param.psName);
    roughness = param.roughness;
    metallic = param.metallic;
}

Material::~Material()
{
}
