#include "stdafx.h"
#include "ForwardRenderer.h"

#include "WindowManager.h"

void ForwardRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene) {
    _scene.Refresh(cmd, scene);

    Vector4D ClearColor { 0.7f, 0.7f, 0.7f, 1.0f };
    cmd->ClearRenderTargetView(view->GetOMResource()->GetMainRTV().get(), ClearColor);

    depthRenderer.render(cmd, view, &_scene);
    bassPassRenderer.render(cmd, view, &_scene);

}
