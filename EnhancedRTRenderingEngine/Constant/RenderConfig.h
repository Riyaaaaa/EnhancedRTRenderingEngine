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

enum class TextureFormat {
    RGBA8_UNORM,
    R16_TYPELESS
};

enum TextureUsage : unsigned int {
    STENCIL = 1,
    SHADER_RESOURCE = 1 << 2
};

enum class TextureType {
    Texture2D,
    Texture2DArray,
    TextureCube
};
