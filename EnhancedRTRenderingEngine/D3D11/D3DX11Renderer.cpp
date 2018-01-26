#include "stdafx.h"
#include "D3DX11Renderer.h"
#include "../Resource/ResourceLoader.h"
#include "D3D11DrawElement.h"
#include "Mesh/Primitive/Primitives.h"
#include "WindowManager.h"

typedef Square PrimitiveType;
auto t = new PrimitiveType;

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
}

void D3DX11Renderer::render() {
	if (!mView) {
		return;
	}

	RenderingContext context{ "VertexShader", "PixelShader", CreateVertexLayout<PrimitiveType::Type>(), VertexPrimitiveType::TRIANGLESTRIP };
	D3D11DrawElement<PrimitiveType::Type> element(mView->hpDevice, context, t);

	element.Draw(mView);
	
	mView->hpDXGISwpChain->Present(0, 0);
}
