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

    virtual GISwapChain* CreateSwapChain(const ViewportParam& param) override;
    virtual void SetViewPortSize(const ViewportCfg& cfg) override;

    virtual void OMSetRenderTargets(int numViews, std::vector<GIRenderTargetView*> rtvs, GIDepthStencilView*) override;

    virtual GIRenderTargetView* CreateRenderTargetView(GITexture2D* tex) override;
    virtual GIDepthStencilView* CreateDepthStencilView(GITexture2D* tex) override;
    virtual void ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) override;
    virtual void ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal = 1.0f, float stencilClearVal = 0.0f) override;

    virtual GITexture2D* CreateTexture2D(const TextureParam& param, const std::vector<Texture2D>& textures);
    virtual GIShaderResourceView* CreateShaderResourceView(GITexture2D* tex, TextureFormat format, TextureType type, unsigned int textureNums, unsigned int mipLevels);
    virtual GISamplerState* CreateSamplerState(const SamplerParam& param);

    GIBuffer* CreateBuffer(ResourceType type, unsigned int stride) override;
    GIBuffer* CreateBuffer(ResourceType type, unsigned int stride, void* initPtr, float byteWidth) override;

    virtual void RSSetState(GIRasterizerState* state);

    virtual void UpdateSubresource() {}

    virtual GIPixelShader* CreatePixelShader(RawBinary byteCode) override;
    virtual void PSSetShaderResources(unsigned int slot, GITextureProxyEntity* texture) override;
    virtual void PSSetSamplers(unsigned int slot, GISamplerState* sampler) override;
    virtual void PSSetShader(GIPixelShader* shader) override;
    virtual void PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) override;

    virtual GIVertexShader* CreateVertexShader(RawBinary byteCode) override;
    virtual void VSSetShader(GIVertexShader* shader) override;
    virtual void VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) override;

    virtual void IASetPrimitiveTopology(VertexPrimitiveType primitiveType) override;
    virtual void IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) override;
    virtual void IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) override;

    GIInputLayout* CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) override;
    GIRasterizerState* CreateRasterizerState(RasterizerType type) override;

    virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0) override;
    virtual void Draw(unsigned int vertexCount, unsigned int startIndex) override;

    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const Texture2D & tex) override;
    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) override;

protected:
    ComPtr<ID3D11Device> _device;
    ComPtr<ID3D11DeviceContext> _deviceContext;
};

