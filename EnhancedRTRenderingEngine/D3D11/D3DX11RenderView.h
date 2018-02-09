
#pragma once

#include <d3d11.h>

#include "D3D11DepthStencilTarget.h"
#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

class D3DX11RenderView
{
public:
    bool Initialize(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

    D3DX11RenderView() {};

    bool EnableFullScreen(HWND hWnd);

    Size GetRenderSize() const { return _renderSize; }
    MSAAQualityType GetMSAAQualityType() const { return _type;  }

//protected:
public:
    ComPtr<ID3D11Device> hpDevice;
    ComPtr<ID3D11DeviceContext> hpDeviceContext;
    ComPtr<IDXGIDevice1> hpDXGI;
    ComPtr<IDXGIAdapter> hpAdapter;
    ComPtr<IDXGIFactory> hpDXGIFactory;
    ComPtr<ID3D11Texture2D> hpBackBuffer;
    ComPtr<ID3D11RenderTargetView> hpRenderTargetView;
    ComPtr<IDXGISwapChain> hpDXGISwpChain;
    ComPtr<ID3D11Texture2D> hpTexture2dDepth;
    ComPtr<ID3D11DepthStencilView> hpDepthStencilView;
    ComPtr<ID3D11RasterizerState> mRasterizerState;
    ComPtr<ID3D11RasterizerState> mDoubleSidedRasterizerState;

    D3D11DepthStencilTarget hpShadowMapTarget;

private:
    MSAAQualityType _type;
    Size _renderSize;
};

