#pragma once

#include <memory>

#include "RenderScene.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11DepthRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);

protected:
    void RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);
    void RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);
};

