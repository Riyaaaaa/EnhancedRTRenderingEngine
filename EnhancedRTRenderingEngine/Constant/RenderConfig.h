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

enum class ResourceUsage {
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

enum class BlendType
{
    ZERO = 1,
    ONE = 2,
    SRC_COLOR = 3,
    INV_SRC_COLOR = 4,
    SRC_ALPHA = 5,
    INV_SRC_ALPHA = 6,
    DEST_ALPHA = 7,
    INV_DEST_ALPHA = 8,
    DEST_COLOR = 9,
    INV_DEST_COLOR = 10,
    SRC_ALPHA_SAT = 11,
    BLEND_FACTOR = 14,
    INV_BLEND_FACTOR = 15,
    SRC1_COLOR = 16,
    INV_SRC1_COLOR = 17,
    SRC1_ALPHA = 18,
    INV_SRC1_ALPHA = 19,
};

enum class BlendTypeOp
{
    ADD = 1,
    SUBTRACT = 2,
    REV_SUBTRACT = 3,
    MIN = 4,
    MAX = 5,
};

enum ColorWriteEnable
{
    COLOR_WRITE_ENABLE_RED = 1,
    COLOR_WRITE_ENABLE_GREEN = 2,
    COLOR_WRITE_ENABLE_BLUE = 4,
    COLOR_WRITE_ENABLE_ALPHA = 8,
    COLOR_WRITE_ENABLE_ALL = (COLOR_WRITE_ENABLE_RED | COLOR_WRITE_ENABLE_GREEN | COLOR_WRITE_ENABLE_BLUE | COLOR_WRITE_ENABLE_ALPHA),
};

