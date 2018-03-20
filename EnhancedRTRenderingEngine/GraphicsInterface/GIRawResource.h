#pragma once

#include <memory>

#include "Constant/GraphicConstants.h"
#include "Resource/RawBinary.h"
#include "Resource/Texture2D.h"

class GIRawResource
{
public:
    GIRawResource(const RawBinary& res, ResourceType type, float stride);

    unsigned int _structureByteStride;
    ResourceType _type;
    RawBinary _resource;
};

class GITextureResource
{
public:
    GITextureResource(const Texture2D& tex);
    GITextureResource(const std::vector<Texture2D>& tex, ResourceType type);

    unsigned int _structureByteStride;
    ResourceType _type;
    std::vector<Texture2D> _resource;
};

