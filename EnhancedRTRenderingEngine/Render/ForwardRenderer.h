#pragma once

#include <memory>
#include <vector>

#include "Structure/Structure.h"
#include "Scene/Scene.h"

#include "RenderScene.h"
#include "DepthRenderer.h"
#include "BasePassRenderer.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class ForwardRenderer
{
public:
    ForwardRenderer(GIImmediateCommands* cmd) :
        bassPassRenderer(cmd),
        depthRenderer(cmd) {}
    void render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);

protected:
    D3D11DepthRenderer depthRenderer;
    D3D11BasePassRenderer bassPassRenderer;
};

