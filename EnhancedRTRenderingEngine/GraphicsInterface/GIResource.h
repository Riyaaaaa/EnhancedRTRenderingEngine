#pragma once

#include <memory>
#include <vector>

#include "Resource/RawBinary.h"
#include "Constant/GraphicConstants.h"
#include "RenderingContext.h"

template<class T>
std::shared_ptr<T> MakeRef(T* ptr) {
    return std::shared_ptr<T>(ptr);
}

class GIResource {};

class GIBuffer : public GIResource
{
public:
    unsigned int structureByteStride;
    ResourceType type;
};

class GITexture2D : public GIResource {
public:
    virtual TextureParam GetTextureParam() = 0;
};

class GISamplerState : public GIResource {};
class GIShaderResourceView : public GIResource {};

class GIRenderTargetView : public GIResource {
public:
    std::shared_ptr<GITexture2D> rtvTexture;
};
class GIDepthStencilView : public GIResource {
public:
    std::shared_ptr<GITexture2D> dsvTexture;
};

class GIOMResource : public GIResource
{
public:
    const std::shared_ptr<GIRenderTargetView>& GetMainRTV() const {
        return renderTargets[0];
    }
    std::vector<std::shared_ptr<GIRenderTargetView>> renderTargets;
    std::shared_ptr<GIDepthStencilView> depthStencilView;
};

class GIRasterizerState : public GIResource {};

class GISwapChain : public GIResource {
public:
    virtual void Present(unsigned int syncInterval, unsigned int flag) = 0;
    std::shared_ptr<GITexture2D> backBuffer;
};

class GIShader : public GIResource
{
public:
    RawBinary byteCode;
};
class GIPixelShader : public GIShader {};
class GIVertexShader : public GIShader {};

class GIInputLayout : public GIResource {};
