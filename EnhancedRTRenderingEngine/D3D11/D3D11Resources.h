#pragma once

#include "D3D11/ComPtr.h"
#include "GraphicsInterface/GIResource.h"

template<class Derived, class T>
auto CastRes(T* res) 
->const decltype(static_cast<Derived*>(res)->resource) & {
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

class D3D11InputLayout : public GIInputLayout {
public:
    ComPtr<ID3D11InputLayout> resource;
};

