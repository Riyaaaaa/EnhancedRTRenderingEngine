#pragma once

#include "RenderCore.D3D11/ComPtr.h"
#include "GraphicsInterface/GIResource.h"

template<class Derived, class T>
auto CastRes(T* res) 
->decltype(static_cast<Derived*>(res)->resource) & {
    return static_cast<Derived*>(res)->resource;
}

template<class Derived, class T>
auto NullableCastRes(T* res)
-> decltype(static_cast<Derived*>(res)->resource) {
    if (!res) {
        return decltype(static_cast<Derived*>(res)->resource)();
    }
    return static_cast<Derived*>(res)->resource;
}

class D3D11Buffer : public GIBuffer {
public:
    ComPtr<ID3D11Buffer> resource;
};

class D3D11RenderTargetView : public GIRenderTargetView {
public:
    ComPtr<ID3D11RenderTargetView> resource;
};

class D3D11DepthStencilView : public GIDepthStencilView {
public:
    ComPtr<ID3D11DepthStencilView> resource;
};

class D3D11RasterizerState : public GIRasterizerState {
public:
    ComPtr<ID3D11RasterizerState> resource;
};

class D3D11SwapChain : public GISwapChain {
public:
    virtual void Present(unsigned int syncInterval, unsigned int flag) override {
        resource->Present(syncInterval, flag);
    }

    ComPtr<IDXGISwapChain> resource;
    ComPtr<IDXGIDevice1> dxgiDevice;
    ComPtr<IDXGIAdapter> adapter;
    ComPtr<IDXGIFactory> factory;
};

class D3D11SamplerState : public GISamplerState {
public:
    ComPtr<ID3D11SamplerState> resource;
};

class D3D11PixelShader : public GIPixelShader {
public:
    ComPtr<ID3D11PixelShader> resource;
};

class D3D11VertexShader : public GIVertexShader {
public:
    ComPtr<ID3D11VertexShader> resource;
};

class D3D11GeometryShader : public GIGeometryShader {
public:
    ComPtr<ID3D11GeometryShader> resource;
};

class D3D11InputLayout : public GIInputLayout {
public:
    ComPtr<ID3D11InputLayout> resource;
};

class D3D11Texture2D : public GITexture2D {
public:
    virtual TextureParam GetTextureParam() override;
    ComPtr<ID3D11Texture2D> resource;
};

class D3D11ShaderResourceView : public GIShaderResourceView {
public:
    ComPtr<ID3D11ShaderResourceView> resource;
};





