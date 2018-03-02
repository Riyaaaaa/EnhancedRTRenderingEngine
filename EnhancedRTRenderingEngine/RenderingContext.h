#pragma once

#include <string>
#include <d3d11.h>

#include <vector>

#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

struct VertexLayout {
    const char* name;
    VertexProperty vProperty;
};

struct RenderingContext
{
    std::vector<VertexLayout> layouts;
    VertexPrimitiveType pType;
};

template<class VertType>
std::vector<VertexLayout> CreateVertexLayout();

struct TextureParam {
    TextureFormat format;
    unsigned int bindFlag;
    TextureType type = TextureType::Texture2D;
    TextureUsage usage = TextureUsage::Default;
    ResourceAccessFlag accessFlag = ResourceAccessFlag::None;
    // ...add as neecesary
};


