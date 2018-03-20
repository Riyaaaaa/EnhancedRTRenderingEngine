#include "stdafx.h"
#include "GIDrawMesh.h"

void GIDrawElement::RegisterShaderResource(const GITextureProxy& tex, unsigned int registerId) {
    textureResources.push_back(std::make_pair(tex, registerId));
}

void GIDrawElement::RegisterShaderResource(GIRawResource res, unsigned int registerId) {
    rawResources.push_back(std::make_pair(res, registerId));
}

template<>
std::vector<VertexLayout> GIDrawMesh::GenerateVertexLayout<Vertex3D>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 }, { "TEXCOORD", VertexProperty::FloatRG,0,0,28 } };
}

template<>
std::vector<VertexLayout> GIDrawMesh::GenerateVertexLayout<SimpleVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "COLOR", VertexProperty::FloatRGBA, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> GIDrawMesh::GenerateVertexLayout<TexVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0 }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 } };
}

template<>
std::vector<VertexLayout> GIDrawMesh::GenerateVertexLayout<PMDVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB, 0, 0, 0  }, { "TEXCOORD", VertexProperty::FloatRG, 0, 0, 12 }, { "NORMAL", VertexProperty::FloatRGB, 0,0,20 }, { "COLOR", VertexProperty::FloatRGBA,0,0,32 } };
}

