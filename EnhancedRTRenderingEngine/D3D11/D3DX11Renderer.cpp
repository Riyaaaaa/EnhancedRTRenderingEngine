#include "stdafx.h"
#include "D3DX11Renderer.h"
#include "D3D11DrawElement.h"

#include "../Resource/ResourceLoader.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"
#include "WindowManager.h"

D3DX11Renderer::D3DX11Renderer()
{
}


D3DX11Renderer::~D3DX11Renderer()
{
}

void D3DX11Renderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext) {
	// require deveice input layout initialization

	mView = std::make_unique<D3DX11RenderView>();
	mView->Initialize(WindowManager::getInstance()->getActiveWindow(), device, hpDeviceContext);
}

void D3DX11Renderer::render(Scene* scene) {
	if (!mView) {
		return;
	}

	float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	mView->hpDeviceContext->ClearRenderTargetView(mView->hpRenderTargetView, ClearColor);
	mView->hpDeviceContext->ClearDepthStencilView(mView->hpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//constantバッファ生成
	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* hpConstantBuffer = NULL;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(mView->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
		return;
	}

	ConstantBuffer hConstantBuffer;
	hConstantBuffer.World = XMMatrixTranspose(scene->GetWorldProjection());
	hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
	hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());

	// Only support one light.
	// TODO: UNSAFE CODE!!! Light always requires
	hConstantBuffer.DirectionalLight = scene->GetDirectionalLights()[0].GetDirection();
	hConstantBuffer.PointLight = scene->GetPointLightParams()[0];
	
	mView->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	mView->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

	for (auto && object : scene->GetViewObjects()) {
		D3D11DrawElement<Scene::VertType>(mView->hpDevice, &object).Draw(mView);
	}
	
	mView->hpDXGISwpChain->Present(0, 0);
}
