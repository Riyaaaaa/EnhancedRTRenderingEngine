#include "stdafx.h"
#include "D3DX11Renderer.h"
#include "../Resource/ResourceLoader.h"

D3DX11Renderer::D3DX11Renderer()
{
}


D3DX11Renderer::~D3DX11Renderer()
{
}

void D3DX11Renderer::Initialize(const std::shared_ptr<D3DX11Device>& device, const std::array<Vertex3D, 3>& vData) {
	D3D11_INPUT_ELEMENT_DESC hInElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	D3D11_BUFFER_DESC hBufferDesc;
	hBufferDesc.ByteWidth = sizeof(Vertex3D) * 3;
	hBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	hBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	hBufferDesc.CPUAccessFlags = 0;
	hBufferDesc.MiscFlags = 0;
	hBufferDesc.StructureByteStride = sizeof(float);

	D3D11_SUBRESOURCE_DATA hSubResourceData;
	hSubResourceData.pSysMem = &vData;
	hSubResourceData.SysMemPitch = 0;
	hSubResourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* hpBuffer;
	if (FAILED(device->hpDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))) {
		return;
	}

	UINT hStrides = sizeof(Vertex3D);
	UINT hOffsets = 0;
	device->hpDeviceContext->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);

	device->hpDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11InputLayout* hpInputLayout = NULL;
	ResourceHandle g_vs_main = ResourceLoader::LoadShader("VertexShader");
	if (FAILED(device->hpDevice->CreateInputLayout(hInElementDesc, ARRAYSIZE(hInElementDesc), g_vs_main.get(), g_vs_main.size(), &hpInputLayout))) {
		return;
	}

	device->hpDeviceContext->IASetInputLayout(hpInputLayout);

	ID3D11VertexShader* hpVertexShader;
	if (FAILED(device->hpDevice->CreateVertexShader(g_vs_main.get(), g_vs_main.size(), NULL, &hpVertexShader))) {
		return;
	}

	device->hpDeviceContext->VSSetShader(hpVertexShader, NULL, 0);

	ID3D11PixelShader* hpPixelShader;
	ResourceHandle g_ps_main = ResourceLoader::LoadShader("PixelShader");
	if (FAILED(device->hpDevice->CreatePixelShader(g_ps_main.get(), g_ps_main.size(), NULL, &hpPixelShader))) {
		return;
	}

	mResourceQueue.push_back(std::move(g_ps_main));
	mResourceQueue.push_back(std::move(g_vs_main));

	device->hpDeviceContext->PSSetShader(hpPixelShader, NULL, 0);

	mDevice = device;
}

void D3DX11Renderer::render() {
	if (!mDevice) {
		return;
	}

	mDevice->hpDeviceContext->Draw(3, 0);
	mDevice->hpDXGISwpChain->Present(0, 0);
}
