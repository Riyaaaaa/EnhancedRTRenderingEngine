#pragma once

#include "RenderConfig.h"

enum class RasterizerType {
    Defalt,
    DoubleSided,
    WireFrame
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
