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

    // Shader Resources
    std::vector<std::pair<ResourceType, int>> psShaderResourceIndices;
    std::vector<Texture2D> textures;
    std::vector<TextureCube> textureCubes;
    
    // Shader Buffers
    std::vector<std::pair<void*, unsigned int>> constantBuffers;
    
    VertexLayout _vertexLayout;

    RawBinary _pshader;
    RawBinary _vshader;
    // RawBinary GShader;
};

