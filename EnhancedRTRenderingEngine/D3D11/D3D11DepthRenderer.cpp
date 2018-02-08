#include "stdafx.h"
#include "D3D11DepthRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"

#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

bool D3D11DepthRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
	_view = view;
	return true;
}

void D3D11DepthRenderer::render(Scene* scene)
{
	_view->hpDeviceContext->OMSetRenderTargets(0, nullptr, _view->hpShadowMapTarget.GetDepthStencilView());
	_view->hpDeviceContext->ClearDepthStencilView(_view->hpShadowMapTarget.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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

	hConstantBuffer.View = XMMatrixTranspose(scene->GetDirectionalLightViewProjection());
	hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());

	_view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	_view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

	for (auto && object : scene->GetViewObjects()) {
		D3D11DrawElement<Scene::VertType> element;
		element.Initialize(_view->hpDevice, &object, DepthRenderTag);
		element.Draw(_view);
	}

	SAFE_RELEASE(hpConstantBuffer);
}
