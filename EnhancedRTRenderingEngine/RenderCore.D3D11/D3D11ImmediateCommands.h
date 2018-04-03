#pragma once

#include <d3d11.h>
#include "ComPtr.h"

#include "GraphicsInterface/GIImmediateCommands.h"

class D3D11ImmediateCommands : public GIImmediateCommands
{
public:
    D3D11ImmediateCommands(bool checkDeviceObjectLeaks = false);
    ~D3D11ImmediateCommands();

    virtual GISwapChain* CreateSwapChain(const ViewportParam& param) override;
    virtual void SetViewport(const ViewportCfg& cfg) override;

    virtual void OMSetRenderTargets(const std::vector<std::shared_ptr<GIRenderTargetView>>& renderTargets, std::shared_ptr<GIDepthStencilView> stv) override;

    virtual GIRenderTargetView* CreateRenderTargetView(GITexture2D* tex) override;
    virtual GIDepthStencilView* CreateDepthStencilView(GITexture2D* tex, TextureFormat format) override;
    virtual void ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) override;
    virtual void ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal = 1.0f, float stencilClearVal = 0.0f) override;

    virtual GITexture2D* CreateTexture2D(const TextureParam& param, const std::vector<Texture2D>& textures = std::vector<Texture2D>());
    virtual GIShaderResourceView* CreateShaderResourceView(GITexture2D* tex);
    virtual GISamplerState* CreateSamplerState(const SamplerParam& param);

    virtual GIBuffer* CreateBuffer(ResourceType type, unsigned int stride, float byteWidth, void* initPtr = nullptr) override;

    virtual void RSSetState(GIRasterizerState* state);

    virtual void UpdateSubresource(GIBuffer* buffer, void* srcData, unsigned int srcRowPitch) override;
    virtual void CopyTexture2D(GITexture2D* dst, unsigned int idx, unsigned int mipLevels, GITexture2D* src);
    virtual void CopyTexture2DFromArray(GITexture2D* dst, GITexture2D* src, unsigned int srcIdx, unsigned int srcMipLevels);

    virtual GIPixelShader* CreatePixelShader(RawBinary byteCode) override;
    virtual void PSSetShaderResources(unsigned int slot, GIShaderResourceView* texture) override;
    virtual void PSSetSamplers(unsigned int slot, GISamplerState* sampler) override;
    virtual void PSSetShader(GIPixelShader* shader) override;
    virtual void PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) override;

    virtual void ResetPS() override;

    virtual GIVertexShader* CreateVertexShader(RawBinary byteCode) override;
    virtual void VSSetShader(GIVertexShader* shader) override;
    virtual void VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) override;

    virtual void IASetPrimitiveTopology(VertexPrimitiveType primitiveType) override;
    virtual void IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) override;
    virtual void IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) override;
    virtual void IASetInputLayout(GIInputLayout* layout) override;

    GIInputLayout* CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) override;
    GIRasterizerState* CreateRasterizerState(RasterizerType type) override;

    virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0) override;
    virtual void Draw(unsigned int vertexCount, unsigned int startIndex) override;

    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const Texture2D & tex) override;
    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) override;
    virtual GITextureProxyEntity* CreateTextureProxy(std::shared_ptr<GITexture2D> tex, SamplerParam param) override;

protected:
    bool _checkDeviceObjectLeaks;
    ComPtr<ID3D11Device> _device;
    ComPtr<ID3D11DeviceContext> _deviceContext;
    ComPtr<ID3D11Debug> _pD3dDebug;
};

