#pragma once

#include "CubeReflectionCapture.h"

class StaticCubeReflectionCapture : public CubeReflectionCapture
{
public:
    explicit StaticCubeReflectionCapture(const TextureCube& tex);
    void SetupTexture(GITextureProxy tex) override;

protected:
    TextureCube reflectionSource;
};

