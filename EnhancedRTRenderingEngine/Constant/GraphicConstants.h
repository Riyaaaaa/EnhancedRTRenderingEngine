#pragma once

enum class ResourceType {
    Texture2D,
    TextureCube,
    IndexList,
    VertexList,
    ConstantBuffer
};

enum class ShaderType {
    VShader,
    PShader
};

enum class ShadingType {
    None,
    BasePass,
    Detph,
    Unlit,
    Vertex
};

