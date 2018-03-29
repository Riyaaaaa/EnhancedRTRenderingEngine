#pragma once

#include "Constant/GraphicConstants.h"
#include "GITextureProxy.h"

class GIResource {};

class GIBuffer : public GIResource
{
public:
    unsigned int structureByteStride;
    ResourceType type;
};

class GIRenderTargetView : public GIResource
{
public:
    GIRenderTargetView(bool _useAsShaderResource)
    : useAsShaderResource(_useAsShaderResource) {}
    bool useAsShaderResource;
    GITextureProxy rtvTexture;
};

class GIDepthStencilView : public GIResource
{
public:
    GIDepthStencilView(bool _useAsShaderResource)
        : useAsShaderResource(_useAsShaderResource) {}
    bool useAsShaderResource;
    GITextureProxy dsvTexture;
};

class GIOMResource : public GIResource
{
public:
    virtual GIRenderTargetView** GetRenderTargetViewsRef() = 0;
};

class GITexture2D : public GIResource {};

class GIRasterizerState : public GIResource {};
class GISamplerState : public GIResource {};

class GIShader : public GIResource
{
public:
    RawBinary byteCode;
};
class GIPixelShader : public GIShader {};
class GIVertexShader : public GIShader {};

class GIInputLayout : public GIResource {};
