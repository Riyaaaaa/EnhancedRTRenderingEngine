#include "stdafx.h"
#include "ForwardRenderer.h"

#include "UserData/UserConfig.h"
#include "WindowsApp.h"

void ForwardRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene) {
    Vector4D ClearColor = UserConfig::getInstance()->GetBGColor();
    cmd->ClearRenderTargetView(view->GetOMResource()->GetMainRTV().get(), ClearColor);

    view->SetRasterizerState(cmd, RasterizerState::Default);

    depthRenderer.render(cmd, view, scene);
    bassPassRenderer.render(cmd, view, scene);
}
