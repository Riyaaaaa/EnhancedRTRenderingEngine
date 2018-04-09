#pragma once

#include "RenderConfig.h"

enum class RasterizerType {
    Defalt,
    DoubleSided,
    WireFrame,
    HUD
};

struct ViewportCfg {
    float topLeftX, topLeftY;
    float width, height;
    float minDepth, maxDepth;
};

struct ViewportParam {
    ViewportCfg cfg;
    MSAAQualityType MSAAQuality;
    void* runtimeWindowHandle;
};

struct BufferDesc {
    ResourceUsage usage = ResourceUsage::Default;
    ResourceAccessFlag accessFlag = ResourceAccessFlag::R;
    unsigned int byteWidth;
    unsigned int stride;
};

struct RenderTargetBlendDesc {
    bool BlendEnable = false;
    BlendType SrcBlend = BlendType::ONE;
    BlendType DestBlend = BlendType::ZERO;
    BlendTypeOp BlendOp = BlendTypeOp::ADD;
    BlendType SrcBlendAlpha = BlendType::ONE;
    BlendType DestBlendAlpha = BlendType::ZERO;
    BlendTypeOp BlendOpAlpha = BlendTypeOp::ADD;
    unsigned int RenderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;
};

struct BlendDesc {
    bool AlphaToCoverageEnable = false;
    bool IndependentBlendEnable = false;
    RenderTargetBlendDesc RenderTarget[8];
};

struct ScissorRect {
    long left;
    long top;
    long right;
    long bottom;
};

