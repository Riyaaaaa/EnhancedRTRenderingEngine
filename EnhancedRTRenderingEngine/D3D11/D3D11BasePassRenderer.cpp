#include "stdafx.h"
#include "D3D11BasePassRenderer.h"
#include "D3D11DrawElement.h"

#include "../Resource/ResourceLoader.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"

#include "Constant/RenderTag.h"
#include "WindowManager.h"
#include "Common.h"

using namespace DirectX;

D3D11BasePassRenderer::D3D11BasePassRenderer()
{
}


D3D11BasePassRenderer::~D3D11BasePassRenderer()
{
}

bool D3D11BasePassRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
	_view = view;
	return true; 
}

void D3D11BasePassRenderer::render(Scene* scene) {
	if (!_view) {
		return;
	}

	_view->hpDeviceContext->OMSetRenderTargets(1, &_view->hpRenderTargetView, _view->hpDepthStencilView);

	float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	_view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView, ClearColor);
	_view->hpDeviceContext->ClearDepthStencilView(_view->hpDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	D3D11_BUFFER_DESC bufferDesc;
	ID3D11Buffer* hpConstantBuffer = NULL;
	bufferDesc.ByteWidth = sizeof(ConstantBuffer);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
		return;
	}

	ConstantBuffer hConstantBuffer;
	hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
	hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());
	hConstantBuffer.Shadow = XMMatrixTranspose(scene->GetDirectionalLightViewProjection());

	// Only support one light.
	if (!scene->GetDirectionalLights().empty()) {
		hConstantBuffer.DirectionalLight = scene->GetDirectionalLights()[0].GetDirection();
	}
	else {
		hConstantBuffer.DirectionalLight = Vector4D::Zero();
	}
	if (!scene->GetPointLightParams().empty()) {
		hConstantBuffer.PointLight = scene->GetPointLightParams()[0];
	}
	else {
		hConstantBuffer.PointLight.pos = Vector4D::Zero();
		hConstantBuffer.PointLight.attenuation = Vector4D::Zero();
	}
	
	_view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
	_view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);
	_view->hpDeviceContext->PSSetConstantBuffers(0, 1, &hpConstantBuffer);
	_view->hpDeviceContext->PSSetShaderResources(1, 1, _view->hpShadowMapTarget.GetSRV());

	for (auto && object : scene->GetViewObjects()) {
		D3D11DrawElement<Scene::VertType> element;
		element.Initialize(_view->hpDevice, &object, OpaqueRenderTag);
		element.Draw(_view);
	}
	
	ID3D11ShaderResourceView*   pNullSRV = nullptr;
	_view->hpDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
	_view->hpDeviceContext->PSSetShader(nullptr, nullptr, 0);
	_view->hpDXGISwpChain->Present(0, 0);

	SAFE_RELEASE(hpConstantBuffer);
}
