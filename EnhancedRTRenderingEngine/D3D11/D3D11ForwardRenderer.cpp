#include "stdafx.h"
#include "D3D11ForwardRenderer.h"

#include "WindowManager.h"

D3D11ForwardRenderer::D3D11ForwardRenderer()
{
}


D3D11ForwardRenderer::~D3D11ForwardRenderer()
{
}

void D3D11ForwardRenderer::Initialize(std::shared_ptr<D3D11RenderView> view) {
    _view = view;
    depthRenderer.Initialize(view);
    bassPassRenderer.Initialize(view);
}

void D3D11ForwardRenderer::render(Scene* scene) {
    _scene.Refresh(_view->hpDevice, scene);

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);

    depthRenderer.render(&_scene);
    bassPassRenderer.render(&_scene);

}
