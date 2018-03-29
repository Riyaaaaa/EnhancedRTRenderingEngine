
#pragma once

#include <d3d11.h>

#include "D3D11OMResource.h"
#include "Constant/RenderConfig.h"
#include "Structure/Structure.h"

#include "GraphicsInterface/GIRenderView.h"

class D3D11RenderView : public GIRenderView
{
public:
    bool Initialize(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);
    bool EnableFullScreen(HWND hWnd);

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

    D3D11_VIEWPORT _viewPortCfg;

private:
    MSAAQualityType _type;
    Size _renderSize;
};

