#include "stdafx.h"

#include "D3D11ImmediateCommands.h"
#include "D3D11FormatUtils.h"
#include "D3D11Resources.h"

void D3D11ImmediateCommands::OMSetRenderTargets(int numViews, std::vector<GIRenderTargetView*> rtvs, GIDepthStencilView* dsv) {
    std::vector<ID3D11RenderTargetView*> rtvs_;

    for (auto && rtv : rtvs) {
        rtvs_.push_back(CastRes<D3D11RenderTargetView>(rtv).Get());
    }

    _deviceContext->OMSetRenderTargets(numViews, &rtvs_[0], CastRes<D3D11DepthStencilView>(dsv).Get());
}

void D3D11ImmediateCommands::SetViewPortSize(const ViewportCfg& cfg) {
    D3D11_VIEWPORT d3d11_cfg;

    d3d11_cfg.TopLeftX = cfg.topLeftX;
    d3d11_cfg.TopLeftY = cfg.topLeftY;
    d3d11_cfg.Width = cfg.width;
    d3d11_cfg.Height = cfg.height;
    d3d11_cfg.MinDepth = cfg.minDepth;
    d3d11_cfg.MaxDepth = cfg.maxDepth;

    _deviceContext->RSSetViewports(1, &d3d11_cfg);
}

GIBuffer* D3D11ImmediateCommands::CreateBuffer(ResourceType type, unsigned int stride) {
    return CreateBuffer(type, stride, nullptr, 0);
}

GIBuffer* D3D11ImmediateCommands::CreateBuffer(ResourceType type, unsigned int stride, void* initPtr, float byteWidth) {
    D3D11_BUFFER_DESC bufferDesc;
    D3D11Buffer* buffer = new D3D11Buffer;

    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = stride;
    bufferDesc.ByteWidth = byteWidth;

    D3D11_SUBRESOURCE_DATA srd;
    D3D11_SUBRESOURCE_DATA* resPtr = nullptr;

    if (initPtr) {
        srd.pSysMem = initPtr;
        resPtr = &srd;
    }

    if (FAILED(_device->CreateBuffer(&bufferDesc, resPtr, buffer->resource.ToCreator()))) {
        return nullptr;
    }

    return buffer;
}

GIInputLayout* D3D11ImmediateCommands::CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) {
    std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
    for (auto&& layout : layouts) {
        inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{
            layout.name, layout.semanticsIndex,
            CastToD3D11Format<DXGI_FORMAT>(layout.vProperty),
            layout.slot,
            layout.memoryOffset,
            D3D11_INPUT_PER_VERTEX_DATA, 0 });
    }

    D3D11InputLayout* inputLayout = new D3D11InputLayout;
    _device->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), shader->byteCode.get(), shader->byteCode.size(), inputLayout->resource.ToCreator());
    return inputLayout;
}

void D3D11ImmediateCommands::ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) {
    _deviceContext->ClearRenderTargetView(CastRes<D3D11RenderTargetView>(view).Get(), reinterpret_cast<float*>(&color));
}

void D3D11ImmediateCommands::ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal, float stencilClearVal) {
    _deviceContext->ClearDepthStencilView(CastRes<D3D11DepthStencilView>(view).Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depthClearVal, stencilClearVal);
}

GIPixelShader* D3D11ImmediateCommands::CreatePixelShader(RawBinary byteCode) {
    D3D11PixelShader* shader = new D3D11PixelShader;
    _device->CreatePixelShader(byteCode.get(), byteCode.size(), nullptr, shader->resource.ToCreator());
    return shader;
}

GIVertexShader* D3D11ImmediateCommands::CreateVertexShader(RawBinary byteCode) {
    D3D11VertexShader* shader = new D3D11VertexShader;
    _device->CreateVertexShader(byteCode.get(), byteCode.size(), nullptr, shader->resource.ToCreator());
    return shader;
}

void D3D11ImmediateCommands::PSSetShaderResources(unsigned int slot, GITextureProxyEntity* texture) {
    auto* texture_ = static_cast<D3D11TextureProxyEntity*>(texture);
    _deviceContext->PSGetShaderResources(slot, 1, texture_->GetSubResourceView().Ref());
}

void D3D11ImmediateCommands::PSSetSamplers(unsigned int slot, GISamplerState* sampler) {
    _deviceContext->PSSetSamplers(slot, 1, CastRes<D3D11SamplerState>(sampler).Ref());
}

void D3D11ImmediateCommands::PSSetShader(GIPixelShader* shader) {
    _deviceContext->PSSetShader(CastRes<D3D11PixelShader>(shader).Get(), nullptr, 0);
}

void D3D11ImmediateCommands::VSSetShader(GIVertexShader* shader) {
    _deviceContext->VSSetShader(CastRes<D3D11VertexShader>(shader).Get(), nullptr, 0);
}

void D3D11ImmediateCommands::PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) {
    _deviceContext->PSSetConstantBuffers(slot, 1, CastRes<D3D11Buffer>(buffer).Ref());
}

void D3D11ImmediateCommands::VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) {
    _deviceContext->VSSetConstantBuffers(slot, 1, CastRes<D3D11Buffer>(buffer).Ref());
}

GIRasterizerState* D3D11ImmediateCommands::CreateRasterizerState(RasterizerType type) {
    D3D11RasterizerState* state = new D3D11RasterizerState;
    CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);

    switch (type) {
    case RasterizerType::Defalt:
        break;
    case RasterizerType::DoubleSided:
        desc.CullMode = D3D11_CULL_NONE;
        break;
    case RasterizerType::WireFrame:
        desc.CullMode = D3D11_CULL_NONE;
        desc.FillMode = D3D11_FILL_WIREFRAME;
        break;
    }
    
    _device->CreateRasterizerState(&desc, state->resource.ToCreator());
    return state;
}


void D3D11ImmediateCommands::IASetPrimitiveTopology(VertexPrimitiveType primitiveType) {
    _deviceContext->IASetPrimitiveTopology(CastToD3D11Format<D3D_PRIMITIVE_TOPOLOGY>(primitiveType));
}

void D3D11ImmediateCommands::IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) {
    _deviceContext->IASetIndexBuffer(CastRes<D3D11Buffer>(buffer).Get(), DXGI_FORMAT_R16_UINT, 0);
}

void D3D11ImmediateCommands::IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) {
    _deviceContext->IASetVertexBuffers(0, 1, CastRes<D3D11Buffer>(buffer).Ref(), &stride, &offset);
}

D3D11TextureProxyEntity* D3D11ImmediateCommands::CreateTextureProxy(TextureParam param, const Texture2D & tex) {
    D3D11TextureProxyEntity* proxy = new D3D11TextureProxyEntity(_device);
    proxy->Initialize(param, tex);
    return proxy;
}

D3D11TextureProxyEntity* D3D11ImmediateCommands::CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) {
    D3D11TextureProxyEntity* proxy = new D3D11TextureProxyEntity(_device);
    proxy->Initialize(param, tex);
    return proxy;
}

void D3D11ImmediateCommands::DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0) {
    _deviceContext->DrawIndexed(indexCount, startIndex, baseIndex);
}

void D3D11ImmediateCommands::Draw(unsigned int vertexCount, unsigned int startIndex) {
    _deviceContext->Draw(vertexCount, startIndex);
}

