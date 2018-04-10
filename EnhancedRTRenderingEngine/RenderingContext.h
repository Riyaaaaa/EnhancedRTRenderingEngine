#pragma once

#include <string>
#include <d3d11.h>

#include <vector>

#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

struct VertexLayout {
    VertexLayout() = default;
    VertexLayout(const char* name, VertexProperty vProperty, unsigned int semanticsIndex, unsigned int slot, unsigned int memoryOffset) :
        name(name),
        vProperty(vProperty),
        semanticsIndex(semanticsIndex),
        slot(slot),
        memoryOffset(memoryOffset) {}

    const char* name;
    VertexProperty vProperty;
    unsigned int semanticsIndex;
    unsigned int slot;
    unsigned int memoryOffset;
};

UINT GetMemoryBlockSize(VertexProperty prop);

struct SamplerParam {
    TextureAddressMode addressMode = TextureAddressMode::WRAP;
};

struct TextureParam {
    unsigned int width, height;
    TextureFormat format = TextureFormat::RGBA8_UNORM;
    unsigned int mipLevels = 1;
    unsigned int bindFlag = TextureBindTarget::SHADER_RESOURCE;
    unsigned int arraySize = 1;
    bool isMultiSampling = false;
    TextureType type = TextureType::Texture2D;
    ResourceUsage usage = ResourceUsage::Default;
    ResourceAccessFlag accessFlag = ResourceAccessFlag::None;  
    SamplerParam samplerParam;
    // ...add as neecesary
};


