#pragma once

#include "GraphicsInterface/GIImmediateCommands.h"
#include "Render/RenderScene.h"

class PrecomputeLightingSystem
{
public:
    void Compute(GIImmediateCommands* cmd, RenderScene* scene);
};

