#pragma once

#include <memory>

#include "D3D11SceneInfo.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11DepthRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene);

protected:
    void RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene);
    void RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene);
};

