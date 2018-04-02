#pragma once

#include "CubeReflectionCapture.h"

#include "GraphicsInterface/GIImmediateCommands.h"

class StaticCubeReflectionCapture : public CubeReflectionCapture
{
public:
    explicit StaticCubeReflectionCapture(const TextureCube& tex);
    void SetupTexture(GIImmediateCommands* cmd, GITextureProxy tex) override;

protected:
    TextureCube reflectionSource;
};

