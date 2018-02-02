#include "stdafx.h"
#include "D3D11DepthRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"

#include "Constant/RenderTag.h"

using namespace DirectX;

bool D3D11DepthRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
	_view = view;
	return true;
}

void D3D11DepthRenderer::render(Scene* scene)
{
	_view->hpDeviceContext->OMSetRenderTargets(0, nullptr, _view->hpDepthStencilView);

	float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	_view->hpDeviceContext->ClearDepthStencilView(_view->hpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* hpConstantBuffer = NULL;
	bufferDesc.ByteWidth = sizeof(TransformBufferParam);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
		return;
	}

	TransformBufferParam hConstantBuffer;

	auto& dLights = scene->GetDirectionalLights();
	if (dLights.empty()) {
		return;
	}
	auto lDir = dLights[0].GetDirection();
	DirectX::XMVECTOR pos = XMVectorSet(0.0f, 1000, 0.0f, 0.0f);
	DirectX::XMVECTOR dir = XMVectorSet(lDir.x, lDir.y, lDir.z, 0.0f);
	DirectX::XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	hConstantBuffer.View = XMMatrixLookToLH(pos, dir, hUp);

	hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());

	_view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	_view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

	for (auto && object : scene->GetViewObjects()) {
		D3D11DrawElement<Scene::VertType> element;
		element.Initialize(_view->hpDevice, &object, DepthRenderTag);
		element.Draw(_view);
	}
}
