#pragma once

#include <d3d11.h>

#include <memory>

#include "D3D11TextureProxy.h"
#include "D3D11SceneInfo.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11BasePassRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene);
};

