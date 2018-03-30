#pragma once

#include "Structure/Structure.h"

#include "GIResource.h"
#include "Constant/GICommandParameter.h"
#include "GraphicsInterface/GITextureProxy.h"

class GIImmediateCommands
{
public:
    virtual GISwapChain* CreateSwapChain(const ViewportParam& param) = 0;
    virtual void SetViewPortSize(const ViewportCfg& cfg) = 0;

    virtual void OMSetRenderTargets(int numViews, std::vector<GIRenderTargetView*> rtvs, GIDepthStencilView*) = 0;

    virtual GIRenderTargetView* CreateRenderTargetView(GITexture2D* tex) = 0;
    virtual GIDepthStencilView* CreateDepthStencilView(GITexture2D* tex) = 0;

    virtual void ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) = 0;
    virtual void ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal = 1.0f, float stencilClearVal = 0.0f) = 0;

    virtual GIBuffer* CreateBuffer(ResourceType type, unsigned int stride) = 0;
    virtual GIBuffer* CreateBuffer(ResourceType type, unsigned int stride, void* initPtr, float byteWidth) = 0;

    virtual GITexture2D* CreateTexture2D(const TextureParam& param, const std::vector<Texture2D>& textures) = 0;
    virtual GIShaderResourceView* CreateShaderResourceView(GITexture2D* tex, TextureFormat format, TextureType type, unsigned int textureNums, unsigned int mipLevels) = 0;
    virtual GISamplerState* CreateSamplerState(const SamplerParam& param) = 0;

    virtual void RSSetState(GIRasterizerState* state) = 0;

    virtual GIPixelShader* CreatePixelShader(RawBinary byteCode) = 0;
    virtual void PSSetShaderResources(unsigned int slot, GITextureProxyEntity* texture) = 0;
    virtual void PSSetSamplers(unsigned int slot, GISamplerState* sampler) = 0;
    virtual void PSSetShader(GIPixelShader* shader) = 0;
    virtual void PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) = 0;

    virtual GIVertexShader* CreateVertexShader(RawBinary byteCode) = 0;
    virtual void VSSetShader(GIVertexShader* shader) = 0;
    virtual void VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) = 0;

    virtual void IASetPrimitiveTopology(VertexPrimitiveType primitiveType) = 0;
    virtual void IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) = 0;
    virtual void IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) = 0;

    virtual GIInputLayout* CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) = 0;
    virtual GIRasterizerState* CreateRasterizerState(RasterizerType type) = 0;

    virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0) = 0;
    virtual void Draw(unsigned int vertexCount, unsigned int startIndex) = 0;

    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const Texture2D & tex) = 0;
    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) = 0;
};

