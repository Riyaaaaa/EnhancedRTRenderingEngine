#pragma once

#include "CubeReflectionCapture.h"

class StaticCubeReflectionCapture : public CubeReflectionCapture
{
public:
    explicit StaticCubeReflectionCapture(const TextureCube& tex);
    void SetupTexture(GITexture* tex) override;

protected:
    TextureCube reflectionSource;
};

