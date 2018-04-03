#pragma once

#include <memory>

#include "RenderScene.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11DepthRenderer
{
public:
    static void render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);

protected:
    static void RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);
    static void RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene);
};

