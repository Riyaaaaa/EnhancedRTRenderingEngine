#include "stdafx.h"
#include "D3D11RenderView.h"

#include "D3D11FormatUtils.h"
#include "../Common.h"

#include "WindowManager.h"

#pragma comment(lib, "d3d11.lib")


bool D3D11RenderView::Initialize(HWND hWnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext)
{
    hpDevice = device;
    hpDeviceContext = deviceContext;

    if (FAILED(device->QueryInterface(__uuidof(IDXGIDevice1), (void**)hpDXGI.ToCreator()))) {
        return false;
    }

    if (FAILED(hpDXGI->GetAdapter(hpAdapter.ToCreator()))) {
        return false;
    }

    hpAdapter->GetParent(__uuidof(IDXGIFactory), (void**)hpDXGIFactory.ToCreator());
    if (hpDXGIFactory == nullptr) {
        return false;
    }

    _renderSize = WindowManager::getInstance()->GetWindowSize();
    _type = MSAAQualityType::RAW_QUALITY;

    DXGI_SWAP_CHAIN_DESC hDXGISwapChainDesc;
    hDXGISwapChainDesc.BufferDesc.Width = _renderSize.w;
    hDXGISwapChainDesc.BufferDesc.Height = _renderSize.h;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    hDXGISwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    hDXGISwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    hDXGISwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    hDXGISwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    hDXGISwapChainDesc.SampleDesc = CastToD3D11Format<DXGI_SAMPLE_DESC>(_type);
    hDXGISwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    hDXGISwapChainDesc.BufferCount = 1;
    hDXGISwapChainDesc.OutputWindow = hWnd;
    hDXGISwapChainDesc.Windowed = TRUE;
    hDXGISwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    hDXGISwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    D3D11_TEXTURE2D_DESC hTexture2dDesc;
    hTexture2dDesc.Width = hDXGISwapChainDesc.BufferDesc.Width;
    hTexture2dDesc.Height = hDXGISwapChainDesc.BufferDesc.Height;
    hTexture2dDesc.MipLevels = 1;
    hTexture2dDesc.ArraySize = 1;
    hTexture2dDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    hTexture2dDesc.SampleDesc = hDXGISwapChainDesc.SampleDesc;
    hTexture2dDesc.Usage = D3D11_USAGE_DEFAULT;
    hTexture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    hTexture2dDesc.CPUAccessFlags = 0;
    hTexture2dDesc.MiscFlags = 0;
    if (FAILED(hpDevice->CreateTexture2D(&hTexture2dDesc, NULL, hpTexture2dDepth.ToCreator()))) {
        return false;
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC hDepthStencilViewDesc;
    hDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    hDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hDepthStencilViewDesc.Flags = 0;
    if (FAILED(hpDevice->CreateDepthStencilView(hpTexture2dDepth.Get(), &hDepthStencilViewDesc, hpDepthStencilView.ToCreator()))) {
        return false;
    }

    if (FAILED(hpDXGIFactory->CreateSwapChain(device.Get(), &hDXGISwapChainDesc, hpDXGISwpChain.ToCreator()))) {
        return false;
    }

    if (FAILED(hpDXGISwpChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)hpBackBuffer.ToCreator()))) {
        return false;
    }

    if (FAILED(device->CreateRenderTargetView(hpBackBuffer.Get(), NULL, hpRenderTargetView.ToCreator()))) {
        return false;
    }

    _viewPortCfg.TopLeftX = 0;
    _viewPortCfg.TopLeftY = 0;
    _viewPortCfg.Width = _renderSize.w;
    _viewPortCfg.Height = _renderSize.h;
    _viewPortCfg.MinDepth = 0.0f;
    _viewPortCfg.MaxDepth = 1.0f;
    hpDeviceContext->RSSetViewports(1, &_viewPortCfg);

    CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);
    device->CreateRasterizerState(&desc, mRasterizerState.ToCreator());

    desc.CullMode = D3D11_CULL_NONE;
    device->CreateRasterizerState(&desc, mDoubleSidedRasterizerState.ToCreator());

    desc.FillMode = D3D11_FILL_WIREFRAME;
    device->CreateRasterizerState(&desc, mWireFrameRasterizerState.ToCreator());

    hpDeviceContext->RSSetState(mRasterizerState.Get());
    //hpDeviceContext->RSSetState(mDoubleSidedRasterizerState.Get());

    return true;
}

void D3D11RenderView::SetRasterizerState(RasterizerState state) {
    switch (state) {
    case RasterizerState::Default:
        hpDeviceContext->RSSetState(mRasterizerState.Get());
        break;
    case RasterizerState::CullNone:
        hpDeviceContext->RSSetState(mDoubleSidedRasterizerState.Get());
        break;
    case RasterizerState::WireFrame:
        hpDeviceContext->RSSetState(mWireFrameRasterizerState.Get());
        break;
    }
}

void D3D11RenderView::SetViewPortSize(Size size) {
    _viewPortCfg.Width = size.w;
    _viewPortCfg.Height = size.h;
    hpDeviceContext->RSSetViewports(1, &_viewPortCfg);
}

bool D3D11RenderView::EnableFullScreen(HWND hWnd) {
    if (FAILED(hpDXGIFactory->MakeWindowAssociation(hWnd, 0))) {
        return false;
    }
    return true;
}
