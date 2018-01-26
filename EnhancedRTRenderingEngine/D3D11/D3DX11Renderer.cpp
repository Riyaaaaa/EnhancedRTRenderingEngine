#include "stdafx.h"
#include "D3DX11Renderer.h"
#include "../Resource/ResourceLoader.h"
#include "D3D11DrawElement.h"
#include "Mesh/Primitive/Triangle.h"
#include "WindowManager.h"

auto t = new Triangle;

D3DX11Renderer::D3DX11Renderer()
{
}


D3DX11Renderer::~D3DX11Renderer()
{
}

void D3DX11Renderer::Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext) {
	// require deveice input layout initialization

	mView = std::make_unique<D3DX11RenderView>();
	mView->Initialize(WindowManager::getInstance()->getActiveWindow(), device, hpDeviceContext);

	CD3D11_RASTERIZER_DESC desc(D3D11_DEFAULT);
	device->CreateRasterizerState(&desc, &mRasterizerState);

	desc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&desc, &mDoubleSidedRasterizerState);
}

void D3DX11Renderer::render() {
	if (!mView) {
		return;
	}

	std::vector<VertexLayout> layouts = {{"POSITION", VertexProperty::FloatRGB},{ "COLOR", VertexProperty::FloatRGBA },{ "TEXCOORD", VertexProperty::FloatRG } };
	RenderingContext context{ "VertexShader", "PixelShader", layouts, VertexPrimitiveType::TRIANGLELIST };
	D3D11DrawElement<Vertex3D> element(context, t);

	element.Draw(mView);
	
	mView->hpDXGISwpChain->Present(0, 0);
}
