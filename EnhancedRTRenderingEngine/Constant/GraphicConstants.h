#pragma once

enum class ResourceType {
    Texture2D,
    TextureCube,
    IndexList,
    VertexList,
    LightVertexList,
    PSConstantBuffer,
    VSConstantBuffer,
    GSConstantBuffer
};

enum class ShaderType {
    PS,
    VS,
    GS
};

enum class ShadingType {
    None,
    BasePass,
    Detph,
    Unlit,
    Vertex,
    Geometry
};

enum PSRegisterSlots {
    BasePassMaterialBuffer = 1,
    BasePassMainTexture = 10,
    MinTextureColorMainTexture = 0,
    UnlitMainTexture = 0
};

