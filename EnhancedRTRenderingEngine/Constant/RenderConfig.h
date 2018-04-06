#pragma once

constexpr int LIGHT_MAX = 4;

enum class VertexProperty {
    FloatRGBA,
    FloatRGB,
    FloatRG,
    FloatR,
    UnormRGBA,
    NONE = 9999
};

enum class VertexPrimitiveType {
    LINELIST,
    TRIANGLELIST,
    TRIANGLESTRIP,
    NONE = 9999
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
    RGBA8_UNORM_SRGB,
    RGBA16_UNORM,
    D16_UNORM,
    D24_UNORM_S8_UINT,
    R16_TYPELESS,
    R16_UNORM,
    R24G8_TYPELESS,
    R32_TYPELESS,
    R32_UINT
};

enum TextureBindTarget : unsigned int {
    STENCIL = 1,
    SHADER_RESOURCE = 1 << 2,
    RENDER_TARGET = 1 << 3
};

enum class TextureType {
    None,
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

enum class TextureAddressMode {
    WRAP,
    MIRROR,
    CLAMP,
    BORADER,
    NONE = 9999
};

enum class ResourceAccessFlag {
    None,
    R,
    W,
    RW
};

enum class RasterizerState {
    None,
    Default,
    WireFrame,
    CullNone
};

