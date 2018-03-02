#include "stdafx.h"
#include "D3D11ForwardRenderer.h"

#include "WindowManager.h"

D3D11ForwardRenderer::D3D11ForwardRenderer()
{
}


D3D11ForwardRenderer::~D3D11ForwardRenderer()
{
}

void D3D11ForwardRenderer::Initialize(std::shared_ptr<D3DX11RenderView> view) {
    depthRenderer.Initialize(view);
    bassPassRenderer.Initialize(view);
}

void D3D11ForwardRenderer::render(Scene* scene) {

    _scene.Refresh(scene);

    depthRenderer.render(&_scene);
    bassPassRenderer.render(&_scene);

}
