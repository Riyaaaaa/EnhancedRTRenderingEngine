#pragma once

constexpr int LIGHT_MAX = 4;

enum class VertexProperty {
    FloatRGBA,
    FloatRGB,
    FloatRG
};

enum class VertexPrimitiveType {
    TRIANGLELIST,
    TRIANGLESTRIP
};

enum class RenderingState {
    NONE,
    FAILED,
    WRITE_DEPTH,
    RENDER_READIED
};

enum MSAAQualityType {
    RAW_QUALITY
};