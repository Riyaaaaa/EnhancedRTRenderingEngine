#pragma once

#include "Constant/GraphicConstants.h"

#include "Material/Material.h"

#include "Resource/Texture2D.h"
#include "Resource/TextureCube.h"


class GIShader
{
public:
    GIShader(const Material& material);

protected:
    template<class VertType>
    std::vector<VertexLayout> GenerateVertexLayout();

    Material _material;

    std::vector<std::pair<ResourceType, int>> shaderResourceIndices;

    std::vector<Texture2D> textures;
    std::vector<TextureCube> textureCubes;
    
    VertexLayout _vertexLayout;

    RawBinary _pshader;
    RawBinary _vshader;
    // RawBinary GShader;
};

