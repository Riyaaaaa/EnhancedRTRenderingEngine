#pragma once

#include "Constant/GraphicConstants.h"

class GIResource
{
public:
};

class GIBuffer : public GIResource
{
public:
    unsigned int structureByteStride;
    ResourceType type;
};