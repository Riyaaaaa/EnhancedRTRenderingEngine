#pragma once

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

