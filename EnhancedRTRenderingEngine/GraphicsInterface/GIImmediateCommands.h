#pragma once

#include "Structure/Structure.h"

#include "GIResource.h"
#include "Constant/GICommandParameter.h"
#include "GraphicsInterface/GITextureProxy.h"

class GIImmediateCommands
{
public:
    virtual ~GIImmediateCommands() {}

    virtual GISwapChain* CreateSwapChain(const ViewportParam& param) = 0;
    virtual void SetViewport(const ViewportCfg& cfg) = 0;

    virtual void OMSetRenderTargets(const std::vector<std::shared_ptr<GIRenderTargetView>>& renderTargets, std::shared_ptr<GIDepthStencilView> stv) = 0;

    virtual GIRenderTargetView* CreateRenderTargetView(GITexture2D* tex) = 0;
    virtual GIDepthStencilView* CreateDepthStencilView(GITexture2D* tex, TextureFormat format) = 0;

    virtual void ClearRenderTargetView(GIRenderTargetView* view, Vector4D color) = 0;
    virtual void ClearDepthStencilView(GIDepthStencilView* view, float depthClearVal = 1.0f, float stencilClearVal = 0.0f) = 0;

    virtual GIBuffer* CreateBuffer(ResourceType type, BufferDesc desc, void* initPtr = nullptr) = 0;

    virtual GITexture2D* CreateTexture2D(const TextureParam& param, const std::vector<Texture2D>& textures = std::vector<Texture2D>()) = 0;
    virtual GIShaderResourceView* CreateShaderResourceView(GITexture2D* tex) = 0;
    virtual GISamplerState* CreateSamplerState(const SamplerParam& param) = 0;
    
    virtual void UpdateSubresource(GIBuffer* buffer, void* srcData, unsigned int srcRowPitch) = 0;
    virtual void CopyTexture2D(GITexture2D* dst, unsigned int idx, unsigned int mipLevels, GITexture2D* src) = 0;
    virtual void CopyTexture2DFromArray(GITexture2D* dst, GITexture2D* src, unsigned int srcIdx, unsigned int srcMipLevels) = 0;

    virtual void RSSetState(GIRasterizerState* state) = 0;
    virtual void RSSetScissorRect(const ScissorRect& rect) = 0;

    virtual GIPixelShader* CreatePixelShader(RawBinary byteCode) = 0;
    virtual void PSSetShaderResources(unsigned int slot, GIShaderResourceView* texture) = 0;
    virtual void PSSetSamplers(unsigned int slot, GISamplerState* sampler) = 0;
    virtual void PSSetShader(GIPixelShader* shader) = 0;
    virtual void PSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) = 0;

    virtual void ResetPS() = 0;

    virtual GIVertexShader* CreateVertexShader(RawBinary byteCode) = 0;
    virtual void VSSetShader(GIVertexShader* shader) = 0;
    virtual void VSSetConstantBuffers(unsigned int slot, GIBuffer* buffer) = 0;

    virtual GIGeometryShader* CreateGeometryShader(RawBinary byteCode) = 0;
    virtual void GSSetShader(GIGeometryShader* shader) = 0;

    virtual void IASetPrimitiveTopology(VertexPrimitiveType primitiveType) = 0;
    virtual void IASetIndexBuffer(GIBuffer* buffer, unsigned int offset) = 0;
    virtual void IASetVertexBuffer(GIBuffer* buffer, unsigned int stride, unsigned int offset) = 0;
    virtual void IASetInputLayout(GIInputLayout* layout) = 0;

    virtual GIInputLayout* CreateInputLayout(const std::vector<VertexLayout>& layouts, GIVertexShader* shader) = 0;
    virtual GIRasterizerState* CreateRasterizerState(RasterizerType type) = 0;

    virtual GIBlendState* CreateBlendState(BlendDesc desc) = 0;
    virtual void OMSetBlendState(GIBlendState* state, Vector4D blendFactor, unsigned int sampleMask = 0xffffffff) = 0;

    virtual void DrawIndexed(unsigned int indexCount, unsigned int startIndex, unsigned int baseIndex = 0) = 0;
    virtual void Draw(unsigned int vertexCount, unsigned int startIndex) = 0;

    virtual GIMappedResource MapBuffer(GIBuffer* buffer, unsigned int idx, MapType mapType) = 0;
    virtual void UnmapBuffer(GIBuffer* buffer, unsigned int idx) = 0;

    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const Texture2D & tex) = 0;
    virtual GITextureProxyEntity* CreateTextureProxy(TextureParam param, const std::vector<Texture2D> & tex) = 0;
    virtual GITextureProxyEntity* CreateTextureProxy(std::shared_ptr<GITexture2D> tex, SamplerParam param) = 0;
};

