#pragma once

#include <d3d11.h>
#include "ComPtr.h"

#include "GraphicsInterface/GIImmediateCommands.h"

#include "D3D11RenderView.h"

class D3D11ImmediateCommands : public GIImmediateCommands
{
public:
    D3D11ImmediateCommands(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> context) :
        _device(device),
        _deviceContext(context) {}

    virtual void SetViewPortSize(const ViewportCfg& cfg);

    virtual void OMSetRenderTargets(int numViews, std::vector<GIRenderTargetView*> rtvs, GIDepthStencilView*);

    virtual void ClearRenderTargetView(GIRenderTargetView* view, Vector4D color);
    virtual void ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal = 1.0f, float stencilClearVal = 0.0f);

    GIBuffer* CreateBuffer(ResourceType type, unsigned int stride) override;
    GIBuffer* CreateBuffer(ResourceType type, unsigned int stride, void* initPtr, float byteWidth) override;

    virtual void UpdateSubresource() {}

    virtual GIPixelShader* CreatePixelShader(RawBinary byteCode);
    virtual void PSSetShaderResources(unsigned int slot, GITextureProxyEntity* texture);
    virtual void PSSetSamplers(unsigned int slot, GISamplerState* sampler);
    virtual void PSSetShader(GIPixelShader* shader);
    virtual void PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer);

    virtual GIVertexShader* CreateVertexShader(RawBinary byteCode);
    virtual void VSSetShader(GIVertexShader* shader);
    virtual void VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer);

    virtual void IASetPrimitiveTopology(VertexPrimitiveType primitiveType);
    virtual void IASetIndexBuffer(GIBuffer* buffer, unsigned int offset);
    virtual void IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset);

    GIInputLayout* CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader);
    GIRasterizerState* CreateRasterizerState(RasterizerType type);

    virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0);
    virtual void Draw(unsigned int vertexCount, unsigned int startIndex);

    virtual D3D11TextureProxyEntity* CreateTextureProxy(TextureParam param, const Texture2D & tex);
    virtual D3D11TextureProxyEntity* CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex);

protected:
    D3D11RenderView _renderView;
    ComPtr<ID3D11Device> _device;
    ComPtr<ID3D11DeviceContext> _deviceContext;
};

