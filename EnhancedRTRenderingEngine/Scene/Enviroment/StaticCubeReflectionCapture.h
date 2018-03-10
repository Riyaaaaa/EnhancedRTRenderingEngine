#pragma once

#include "CubeReflectionCapture.h"

class StaticCubeReflectionCapture : public CubeReflectionCapture
{
public:
    explicit StaticCubeReflectionCapture(const CubeTexture& tex);
    void SetupTexture(GITexture* tex) override;

protected:
    CubeTexture reflectionSource;
};

