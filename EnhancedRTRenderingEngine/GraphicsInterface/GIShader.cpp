#include "stdafx.h"
#include "GIShader.h"

void GIDrawFace::RegisterShaderResource(const Texture2D& tex, unsigned int registerId, ShaderType type) {
    textureResources.push_back(std::make_pair(GITextureResource(tex), registerId));
}

void GIDrawFace::RegisterShaderResource(const TextureCube& tex, unsigned int registerId, ShaderType type) {
    textureResources.push_back(std::make_pair(GITextureResource(tex.textures, ResourceType::Texture2D), registerId));
}

void GIDrawFace::RegisterShaderResource(GIRawResource res, unsigned int registerId) {
    rawResources.push_back(std::make_pair(res, registerId));
}

template<>
std::vector<VertexLayout> GIDrawElement::GenerateVertexLayout<Vertex3D>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 }, { "TEXCOORD", VertexProperty::FloatRG,0,0,28 } };
}

template<>
std::vector<VertexLayout> GIDrawElement::GenerateVertexLayout<SimpleVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> GIDrawElement::GenerateVertexLayout<TexVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0 }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> GIDrawElement::GenerateVertexLayout<PMDVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 }, { "NORMAL", VertexProperty::FloatRGB, 0,0,20 }, { "COLOR", VertexProperty::FloatRGBA,0,0,32 } };
}

