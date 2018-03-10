#pragma once

#include "CubeReflectionCapture.h"

class StaticCubeReflectionCapture : public CubeReflectionCapture
{
public:
    explicit StaticCubeReflectionCapture(const CubeTexture& tex);

protected:
    CubeTexture reflectionSource;
};

