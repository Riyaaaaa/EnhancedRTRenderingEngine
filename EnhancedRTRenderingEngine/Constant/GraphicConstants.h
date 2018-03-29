#pragma once

enum class ResourceType {
    Texture2D,
    TextureCube,
    IndexList,
    VertexList,
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

