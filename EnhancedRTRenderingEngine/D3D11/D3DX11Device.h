
#pragma once

#include <d3d11.h>

class D3DX11RenderView
{
public:
	bool Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	D3DX11RenderView() {};
	~D3DX11RenderView();

	bool EnableFullScreen(HWND hWnd);

//protected:
public:
	ID3D11Device * hpDevice = NULL;
	ID3D11DeviceContext* hpDeviceContext = NULL;
	IDXGIDevice1* hpDXGI = NULL;
	IDXGIAdapter* hpAdapter = NULL;
	IDXGIFactory* hpDXGIFactory = NULL;
	ID3D11Texture2D* hpBackBuffer = NULL;
	ID3D11RenderTargetView *hpRenderTargetView = NULL;
	IDXGISwapChain* hpDXGISwpChain = NULL;
};

