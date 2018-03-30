#include "stdafx.h"
#include "D3D11ForwardRenderer.h"

#include "WindowManager.h"

void D3D11ForwardRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Scene* scene) {
    _scene.Refresh(_view->hpDevice, scene);

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);

    depthRenderer.render(&_scene);
    bassPassRenderer.render(&_scene);

}
