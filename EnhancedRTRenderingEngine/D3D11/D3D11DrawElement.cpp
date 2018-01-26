#include "stdafx.h"
#include "D3D11DrawElement.h"

#include "D3D11Texture.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"

template<class T, class U = void>
T CastToD3D11Formart(U prop);

template<>
DXGI_FORMAT CastToD3D11Formart<DXGI_FORMAT, VertexProperty>(VertexProperty prop) {
	switch (prop)
	{
	case VertexProperty::FloatRG:
		return DXGI_FORMAT_R32G32_FLOAT;
	case VertexProperty::FloatRGB:
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case VertexProperty::FloatRGBA:
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
		break;
	default:
		return DXGI_FORMAT_UNKNOWN;
	}
}

template<>
D3D_PRIMITIVE_TOPOLOGY CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY, VertexPrimitiveType>(VertexPrimitiveType prop) {
	switch (prop)
	{
	case VertexPrimitiveType::TRIANGLELIST:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case VertexPrimitiveType::TRIANGLESTRIP:
		return D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	default:
		return D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
	}
}

UINT GetMemoryBlockSize(VertexProperty prop) {
	switch (prop)
	{
	case VertexProperty::FloatRG:
		return 4 * 2;
	case VertexProperty::FloatRGB:
		return 4 * 3;
	case VertexProperty::FloatRGBA:
		return 4 * 4;
		break;
	default:
		return 0;
	}
}

template<class VertType>
D3D11DrawElement<VertType>::D3D11DrawElement(ID3D11Device* device, RenderingContext context, MeshBase<VertType>* mesh) {

	UINT memoryOffset = 0;
	for (auto&& layout : context.layouts) {
		inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Formart<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		memoryOffset += GetMemoryBlockSize(layout.vProperty);
	}

	primitiveTopology = CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

	subResource.pSysMem = &mesh->hVectorData[0];
	//subResource.SysMemPitch = 0;
	//subResource.SysMemSlicePitch = 0;

	vertexCount = mesh->vertexCount;
	bufferDesc.ByteWidth = sizeof(VertType) * vertexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	//bufferDesc.StructureByteStride = sizeof(float);

	Texture2D resourceTex;
	ResourceLoader::LoadTexture("test", &resourceTex);
	tex.Initialize(device, resourceTex);

	vShader = ResourceLoader::LoadShader(context.VSName);
	pShader = ResourceLoader::LoadShader(context.PSName);
}

template<class VertType>
void D3D11DrawElement<VertType>::Draw(const std::unique_ptr<D3DX11RenderView>& view) {
	ID3D11Buffer* hpBuffer;
	if (FAILED(view->hpDevice->CreateBuffer(&bufferDesc, &subResource, &hpBuffer))) {
		return;
	}

	UINT hStrides = sizeof(VertType);
	UINT hOffsets = 0;

	view->hpDeviceContext->IASetVertexBuffers(0, 1, &hpBuffer, &hStrides, &hOffsets);
	//view->hpDeviceContext->IASetPrimitiveTopology(primitiveTopology);
	view->hpDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11InputLayout* hpInputLayout = NULL;
	auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), vShader.get(), vShader.size(), &hpInputLayout);
	if (FAILED(err)) {
		return;
	}

	view->hpDeviceContext->IASetInputLayout(hpInputLayout);

	ID3D11VertexShader* hpVertexShader;
	if (FAILED(view->hpDevice->CreateVertexShader(vShader.get(), vShader.size(), NULL, &hpVertexShader))) {
		return;
	}
	view->hpDeviceContext->VSSetShader(hpVertexShader, NULL, 0);

	ID3D11PixelShader* hpPixelShader;
	if (FAILED(view->hpDevice->CreatePixelShader(pShader.get(), pShader.size(), NULL, &hpPixelShader))) {
		return;
	}
	view->hpDeviceContext->PSSetShader(hpPixelShader, NULL, 0);

	view->hpDeviceContext->PSSetShaderResources(0, 1, tex.GetSubResourceViewRef());
	view->hpDeviceContext->PSSetSamplers(0, 1, tex.GetSamplerRef());

	view->hpDeviceContext->Draw(vertexCount, 0);
}

template D3D11DrawElement<Vertex3D>;
template D3D11DrawElement<SimpleVertex>;
