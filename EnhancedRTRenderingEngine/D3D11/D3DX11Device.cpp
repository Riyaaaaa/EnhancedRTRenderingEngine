#include "stdafx.h"
#include "D3DX11Device.h"

#include "../Common.h"

#pragma comment(lib, "d3d11.lib")


bool D3DX11RenderView::Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	hpDevice = device;
	hpDeviceContext = deviceContext;

	if (FAILED(device->QueryInterface(__uuidof(IDXGIDevice1), (void**)&hpDXGI))) {
		return false;
	}

	if (FAILED(hpDXGI->GetAdapter(&hpAdapter))) {
		return false;
	}

	hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&hpDXGIFactory);
	if (hpDXGIFactory == NULL) {
		return false;
	}

	DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
	hDXGISwapChainDesc.BufferDesc.Width = 1980;
	hDXGISwapChainDesc.BufferDesc.Height = 1080;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	hDXGISwapChainDesc.SampleDesc.Count = 1;
	hDXGISwapChainDesc.SampleDesc.Quality = 0;
	hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	hDXGISwapChainDesc.BufferCount = 1;
	hDXGISwapChainDesc.OutputWindow = hWnd;
	hDXGISwapChainDesc.Windowed = TRUE;
	hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(hpDXGIFactory->CreateSwapChain(device, &hDXGISwapChainDesc, &hpDXGISwpChain))) {
		return false;
	}

	if (FAILED(hpDXGISwpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&hpBackBuffer))) {
		return false;
	}

	if (FAILED(device->CreateRenderTargetView(hpBackBuffer, NULL, &hpRenderTargetView))) {
		return false;
	}

	hpDeviceContext->OMSetRenderTargets(1, &hpRenderTargetView, NULL);

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	hpDeviceContext->RSSetViewports(1, &vp);

	return true;
}

bool D3DX11RenderView::EnableFullScreen(HWND hWnd) {
	if (FAILED(hpDXGIFactory->MakeWindowAssociation(hWnd, 0))) {
		return false;
	}
	return true;
}


D3DX11RenderView::~D3DX11RenderView()
{
	SAFE_RELEASE(hpRenderTargetView);
	SAFE_RELEASE(hpBackBuffer);
	SAFE_RELEASE(hpDXGISwpChain);
	SAFE_RELEASE(hpDXGIFactory);
	SAFE_RELEASE(hpAdapter);
	SAFE_RELEASE(hpDXGI);
	SAFE_RELEASE(hpDeviceContext);
}
