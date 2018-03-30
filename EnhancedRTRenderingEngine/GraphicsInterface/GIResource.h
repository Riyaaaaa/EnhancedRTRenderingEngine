#pragma once

#include "Constant/GraphicConstants.h"
#include "GITextureProxy.h"

template<class T>
std::shared_ptr<T> MakeRef(T* ptr) {
    return std::make_shared<T>(ptr);
}

class GIResource {};

class GIBuffer : public GIResource
{
public:
    unsigned int structureByteStride;
    ResourceType type;
};

class GIRenderTargetView : public GIResource {};

class GIDepthStencilView : public GIResource {};

class GIOMResource : public GIResource
{
public:
    std::vector<std::shared_ptr<GIRenderTargetView>> renderTargets;
    std::shared_ptr<GIDepthStencilView> depthStencilView;
};

class GISwapChain : public GIResource {
public:
    std::shared_ptr<GITexture2D> backBuffer;
};

class GITexture2D : public GIResource {};
class GISamplerState : public GIResource {};
class GIShaderResourceView : public GIResource {};

class GIRasterizerState : public GIResource {};


class GIShader : public GIResource
{
public:
    RawBinary byteCode;
};
class GIPixelShader : public GIShader {};
class GIVertexShader : public GIShader {};

class GIInputLayout : public GIResource {};
