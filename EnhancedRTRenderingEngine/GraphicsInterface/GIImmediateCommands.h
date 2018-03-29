#pragma once

#include "Structure/Structure.h"

#include "GIResource.h"
#include "Constant/GICommandParameter.h"
#include "GraphicsInterface/GITextureProxy.h"

class GIImmediateCommands
{
public:
    virtual void SetViewPortSize(Size size) = 0;
    
    virtual void OMSetRenderTargets() = 0;
    virtual void ClearRenderTargetView() = 0;
    virtual void ClearDepthStencilView() = 0;

    virtual GIBuffer* CreateBuffer(ResourceType type, unsigned int stride) = 0;
    virtual GIBuffer* CreateBuffer(ResourceType type, unsigned int stride, void* initPtr, float byteWidth) = 0;

    virtual void UpdateSubresource() = 0;

    virtual void CreatePixelShader() = 0;
    virtual void PSSetShaderResources() = 0;
    virtual void PSSetSamplers() = 0;
    virtual void PSSetShader() = 0;
    virtual void PSSetConstantBuffers() = 0;

    virtual void CreateVertexShader() = 0;
    virtual void VSSetShader() = 0;
    virtual void VSSetConstantBuffers() = 0;

    virtual void IASetPrimitiveTopology() = 0;
    virtual void IASetIndexBuffer() = 0;
    virtual void IASetVertexBuffers() = 0;

    virtual void CreateInputLayout() = 0;

    virtual GIRasterizerState* CreateRasterizerState(RasterizerType type) = 0;

    virtual GITextureProxy CreateTextureProxy() = 0;
    virtual void DrawIndexed() = 0;
    virtual void Draw() = 0;
};

