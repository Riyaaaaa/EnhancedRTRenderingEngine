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
    Material(const MaterialParameters& param, const ResourceHandle<Texture2D>& texture);

    Material& operator=(Material&& other) noexcept {
        if (this != &other) {
            roughness = other.roughness;
            metallic = other.metallic;
            specular = other.specular;
            vShader = std::move(other.vShader);
            pShader = std::move(other.pShader);
            texture = std::move(other.texture);
        }
        return *this;
    }

    Material(Material&& other) noexcept {
        *this = std::move(other);
    }

    float roughness, metallic;
    Vector3D specular;
    ResourceHandle<> vShader, pShader;
    ResourceHandle<Texture2D> texture;
};

