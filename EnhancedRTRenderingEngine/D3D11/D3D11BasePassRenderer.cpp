#include "stdafx.h"
#include "D3D11BasePassRenderer.h"
#include "D3D11DrawElement.h"

#include "../Resource/ResourceLoader.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"
#include "WindowManager.h"

D3D11BasePassRenderer::D3D11BasePassRenderer()
{
}


D3D11BasePassRenderer::~D3D11BasePassRenderer()
{
}

void D3D11BasePassRenderer::render(const std::shared_ptr<D3DX11RenderView>& view, Scene* scene) {
	if (!view) {
		return;
	}

	float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	view->hpDeviceContext->ClearRenderTargetView(view->hpRenderTargetView, ClearColor);
	view->hpDeviceContext->ClearDepthStencilView(view->hpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* hpConstantBuffer = NULL;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
		return;
	}

	ConstantBuffer hConstantBuffer;
	hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
	hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());

	// Only support one light.
	// TODO: UNSAFE CODE!!! Light always requires
	hConstantBuffer.DirectionalLight = scene->GetDirectionalLights()[0].GetDirection();
	hConstantBuffer.PointLight = scene->GetPointLightParams()[0];
	
	view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

	for (auto && object : scene->GetViewObjects()) {
		D3D11DrawElement<Scene::VertType>(view->hpDevice, &object).Draw(view);
	}
	
	view->hpDXGISwpChain->Present(0, 0);
}
