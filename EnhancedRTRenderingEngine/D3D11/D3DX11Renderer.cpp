#include "stdafx.h"
#include "D3DX11Renderer.h"
#include "../Resource/ResourceLoader.h"
#include "D3D11DrawElement.h"
#include "Mesh/Primitive/Triangle.h"

auto t = new Triangle;

D3DX11Renderer::D3DX11Renderer()
{
}


D3DX11Renderer::~D3DX11Renderer()
{
}

void D3DX11Renderer::Initialize(const std::shared_ptr<D3DX11Device>& device) {
	mDevice = device;
}

void D3DX11Renderer::render() {
	if (!mDevice) {
		return;
	}

	RenderingContext context{ "VertexShader", "PixelShader", VertexProperty::FloatRGBA, VertexPrimitiveType::TRIANGLELIST };
	D3D11DrawElement<Vertex3D> element(context, t);

	element.Draw(mDevice);
	
	mDevice->hpDXGISwpChain->Present(0, 0);
}
