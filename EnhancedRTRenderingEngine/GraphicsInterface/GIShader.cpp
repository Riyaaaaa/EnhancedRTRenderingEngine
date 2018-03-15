#include "stdafx.h"
#include "GIShader.h"


GIShader::GIShader(const Material& material) :
    _material(material)
{

}


template<>
std::vector<VertexLayout> GIShader::GenerateVertexLayout<Vertex3D>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "COLOR", VertexProperty::FloatRGBA }, { "TEXCOORD", VertexProperty::FloatRG } };
}

template<>
std::vector<VertexLayout> GIShader::GenerateVertexLayout<SimpleVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "COLOR", VertexProperty::FloatRGBA } };
}

template<>
std::vector<VertexLayout> GIShader::GenerateVertexLayout<TexVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "TEXCOORD", VertexProperty::FloatRG } };
}

template<>
std::vector<VertexLayout> GIShader::GenerateVertexLayout<PMDVertex>() {
    return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "TEXCOORD", VertexProperty::FloatRG }, { "NORMAL", VertexProperty::FloatRGB }, { "COLOR", VertexProperty::FloatRGBA } };
}

