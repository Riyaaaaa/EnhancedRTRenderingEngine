#include "stdafx.h"
#include "D3D11DepthRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"

using namespace DirectX;

bool D3D11DepthRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
	ID3D11Texture2D* hpTexture2dDepth = NULL;
	D3D11_TEXTURE2D_DESC hTexture2dDesc;
	hTexture2dDesc.Width = view->GetRenderSize().w;
	hTexture2dDesc.Height = view->GetRenderSize().h;
	hTexture2dDesc.MipLevels = 1;
	hTexture2dDesc.ArraySize = 1;
	hTexture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	hTexture2dDesc.SampleDesc = CastToD3D11Formart<DXGI_SAMPLE_DESC>(view->GetMSAAQualityType());
	hTexture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	hTexture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hTexture2dDesc.CPUAccessFlags = 0;
	hTexture2dDesc.MiscFlags = 0;
	if (FAILED(view->hpDevice->CreateTexture2D(&hTexture2dDesc, NULL, &hpTexture2dDepth))) {
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
	hDepthStencilViewDesc.Format = hTexture2dDesc.Format;
	hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	hDepthStencilViewDesc.Flags = 0;
	if (FAILED(view->hpDevice->CreateDepthStencilView(hpTexture2dDepth, &hDepthStencilViewDesc, &lightDepthView))) {
		return false;
	}

	_view = view;
	return true;
}

void D3D11DepthRenderer::render(Scene* scene)
{
	_view->hpDeviceContext->OMSetRenderTargets(0, nullptr, lightDepthView);

	float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	_view->hpDeviceContext->ClearDepthStencilView(lightDepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

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
		D3D11DrawElement<Scene::VertType>(_view->hpDevice, &object).Draw(_view);
	}
}
