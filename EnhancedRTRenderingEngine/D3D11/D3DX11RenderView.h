
#pragma once

#include <d3d11.h>

#include "D3D11DepthStencilTarget.h"
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
	ID3D11Device * hpDevice = nullptr;
	ID3D11DeviceContext* hpDeviceContext = nullptr;
	IDXGIDevice1* hpDXGI = nullptr;
	IDXGIAdapter* hpAdapter = nullptr;
	IDXGIFactory* hpDXGIFactory = nullptr;
	ID3D11Texture2D* hpBackBuffer = nullptr;
	ID3D11RenderTargetView *hpRenderTargetView = nullptr;
	IDXGISwapChain* hpDXGISwpChain = nullptr;
	ID3D11Texture2D* hpTexture2dDepth = nullptr;
	ID3D11DepthStencilView* hpDepthStencilView = nullptr;
	ID3D11RasterizerState* mRasterizerState = nullptr;
	ID3D11RasterizerState* mDoubleSidedRasterizerState = nullptr;

	D3D11DepthStencilTarget hpShadowMapTarget;

private:
	MSAAQualityType _type;
	Size _renderSize;
};

