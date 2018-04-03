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

class D3D11ForwardRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene);

    D3D11SceneInfo& GetSceneInfo() { return _scene; }

protected:
    D3D11SceneInfo _scene;
    D3D11DepthRenderer depthRenderer;
    D3D11BasePassRenderer bassPassRenderer;
};

