#include "stdafx.h"
#include "RenderingContext.h"

template<>
std::vector<VertexLayout> CreateVertexLayout<Vertex3D>() {
	return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "COLOR", VertexProperty::FloatRGBA }, { "TEXCOORD", VertexProperty::FloatRG } };
}

template<>
std::vector<VertexLayout> CreateVertexLayout<SimpleVertex>() {
	return std::vector<VertexLayout>{ { "POSITION", VertexProperty::FloatRGB }, { "COLOR", VertexProperty::FloatRGBA } };
}