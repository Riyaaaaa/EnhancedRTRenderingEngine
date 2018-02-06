#include "stdafx.h"
#include "D3D11ForwardRenderer.h"

#include "WindowManager.h"


D3D11ForwardRenderer::D3D11ForwardRenderer()
{
}


D3D11ForwardRenderer::~D3D11ForwardRenderer()
{
}

void D3D11ForwardRenderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext) {
	mView = std::make_shared<D3DX11RenderView>();
	mView->Initialize(WindowManager::getInstance()->GetActiveWindow(), device, hpDeviceContext);

	depthRenderer.Initialize(mView);
	bassPassRenderer.Initialize(mView);
}

void D3D11ForwardRenderer::render(Scene* scene) {
	depthRenderer.render(scene);
	bassPassRenderer.render(scene);
}
