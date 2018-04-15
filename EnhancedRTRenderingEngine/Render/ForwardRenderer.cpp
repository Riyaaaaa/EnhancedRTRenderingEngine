#include "stdafx.h"
#include "ForwardRenderer.h"

#include "UserData/UserConfig.h"
#include "WindowsApp.h"

void ForwardRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene) {
    _scene.Refresh(cmd, scene);

    Vector4D ClearColor = UserConfig::getInstance()->GetBGColor();
    cmd->ClearRenderTargetView(view->GetOMResource()->GetMainRTV().get(), ClearColor);

    view->SetRasterizerState(cmd, RasterizerState::Default);

    depthRenderer.render(cmd, view, &_scene);
    bassPassRenderer.render(cmd, view, &_scene);
}
