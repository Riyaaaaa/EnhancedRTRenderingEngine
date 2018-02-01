
#pragma once

#include <d3d11.h>

#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

class D3DX11RenderView
{
public:
	bool Initialize(HWND hWnd, ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	D3DX11RenderView() {};
	~D3DX11RenderView();

	bool EnableFullScreen(HWND hWnd);

	Size GetRenderSize() const { return _renderSize; }
	MSAAQualityType GetMSAAQualityType() const { return _type;  }

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
	ID3D11DepthStencilView* hpDepthStencilView = NULL;
	ID3D11RasterizerState* mRasterizerState;
	ID3D11RasterizerState* mDoubleSidedRasterizerState;

private:
	MSAAQualityType _type;
	Size _renderSize;
};

