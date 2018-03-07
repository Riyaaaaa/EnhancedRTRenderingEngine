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
    WRITE_HUD,
    RENDER_READIED
};

enum MSAAQualityType {
    RAW_QUALITY
};

enum class TextureFormat {
    RGBA8_UNORM,
    R16_TYPELESS
};

enum TextureBindTarget : unsigned int {
    STENCIL = 1,
    SHADER_RESOURCE = 1 << 2
};

enum class TextureType {
    Texture2D,
    Texture2DArray,
    TextureCube
};

enum class TextureUsage {
    Default,
    Immutable,
    Dynamic,
    Staging
};

enum class ResourceAccessFlag {
    None,
    R,
    W,
    RW
};
