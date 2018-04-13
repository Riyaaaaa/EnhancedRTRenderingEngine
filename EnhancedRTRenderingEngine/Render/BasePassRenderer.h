#pragma once

#include <memory>

#include "RenderScene.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11BasePassRenderer
{
public:
    D3D11BasePassRenderer(GIImmediateCommands* cmd);
    void render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);

protected:
    std::shared_ptr<GIBuffer> constantBuffer, materialBuffer, objectBuffer;
};

