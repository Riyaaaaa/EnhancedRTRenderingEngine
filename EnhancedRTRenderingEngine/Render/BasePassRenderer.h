#pragma once

#include <memory>

#include "RenderScene.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11BasePassRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene);
};

