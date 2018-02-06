#include "stdafx.h"
#include "D3D11DrawElement.h"

#include "D3D11Texture.h"
#include "D3D11FormatUtils.h"

#include "Structure/Structure.h"
#include "Resource/ResourceLoader.h"
#include "Constant/RenderTag.h"
#include "Common.h"

template<class VertType>
D3D11DrawElement<VertType>::~D3D11DrawElement() {
	SAFE_RELEASE(transformBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ID3D11Device* device, MeshObject<VertType>* element, RenderTag::OpaqueRender) {
	auto& context = element->GetContext();
	_state = RenderingState::NONE;

	UINT memoryOffset = 0;
	for (auto&& layout : context.layouts) {
		inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ layout.name, 0, CastToD3D11Formart<DXGI_FORMAT>(layout.vProperty), 0, memoryOffset, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		memoryOffset += GetMemoryBlockSize(layout.vProperty);
	}

	primitiveTopology = CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

	if (!CreateBuffer(device, element)) {
		_state = RenderingState::FAILED;
		return;
	}

	if (element->GetMaterial().texture().isValid()) {
		tex.Initialize(device, element->GetMaterial().texture);
	}

	vShader = element->GetMaterial().vShader;
	pShader = element->GetMaterial().pShader;

	_state = RenderingState::RENDER_READIED;
}


template<class VertType>
void D3D11DrawElement<VertType>::Initialize(ID3D11Device* device, MeshObject<VertType>* element, RenderTag::DepthRender) {
	auto& context = element->GetContext();
	_state = RenderingState::NONE;

	inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	inElemDesc.push_back(D3D11_INPUT_ELEMENT_DESC{ "NORMAL", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 });

	primitiveTopology = CastToD3D11Formart<D3D_PRIMITIVE_TOPOLOGY>(context.pType);

	if (!CreateBuffer(device, element)) {
		_state = RenderingState::FAILED;
		return;
	}

	vShader = ResourceLoader::LoadShader("DepthVertexShader");

	_state = RenderingState::RENDER_READIED;
}

template<class VertType>
bool D3D11DrawElement<VertType>::CreateBuffer(ID3D11Device* device, MeshObject<VertType>* element) {
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subResource;

	subResource.pSysMem = &(element->GetMesh().GetVertexList()[0]);
	vertexCount = element->GetMesh().GetVertexList().size();
	bufferDesc.ByteWidth = sizeof(VertType) * vertexCount;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	if (FAILED(device->CreateBuffer(&bufferDesc, &subResource, &vertexBuffer))) {
		return false;
	}

	D3D11_SUBRESOURCE_DATA constantSubResource;
	auto mat = XMMatrixTranspose(element->GetMatrix());
	constantSubResource.pSysMem = &mat;
	//D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);
	if (FAILED(device->CreateBuffer(&bufferDesc, &constantSubResource, &transformBuffer))) {
		return false;
	}

	if (element->GetMesh().HasIndexList()) {
		vertexCount = element->GetMesh().GetIndexList().size();
		bufferDesc.ByteWidth = sizeof(element->GetMesh().GetIndexList()[0]) * vertexCount;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = sizeof(unsigned short);

		subResource.pSysMem = &element->GetMesh().GetIndexList()[0];
		subResource.SysMemPitch = 0;
		subResource.SysMemSlicePitch = 0;

		if (FAILED(device->CreateBuffer(&bufferDesc, &subResource, &indexBuffer))) {
			return false;
		}
	}

	return true;
}

template<class VertType>
void D3D11DrawElement<VertType>::SetShader(const std::shared_ptr<D3DX11RenderView>& view) {
	ID3D11InputLayout* hpInputLayout = NULL;
	auto err = view->hpDevice->CreateInputLayout(&inElemDesc[0], inElemDesc.size(), vShader().get(), vShader().size(), &hpInputLayout);
	if (FAILED(err)) {
		return;
	}

	view->hpDeviceContext->IASetInputLayout(hpInputLayout);

	ID3D11VertexShader* hpVertexShader;
	if (FAILED(view->hpDevice->CreateVertexShader(vShader().get(), vShader().size(), NULL, &hpVertexShader))) {
		return;
	}
	view->hpDeviceContext->VSSetShader(hpVertexShader, NULL, 0);

	if (pShader.HasResource()) {
		ID3D11PixelShader* hpPixelShader;
		if (FAILED(view->hpDevice->CreatePixelShader(pShader().get(), pShader().size(), NULL, &hpPixelShader))) {
			return;
		}
		view->hpDeviceContext->PSSetShader(hpPixelShader, NULL, 0);
	}
	else {
		view->hpDeviceContext->PSSetShader(nullptr, NULL, 0);
	}

	if (tex.IsAvalable()) {
		view->hpDeviceContext->PSSetShaderResources(0, 1, tex.GetSubResourceViewRef());
	}
	view->hpDeviceContext->PSSetSamplers(0, 1, tex.GetSamplerRef());
}

template<class VertType>
void D3D11DrawElement<VertType>::SetBuffer(const std::shared_ptr<D3DX11RenderView>& view) {
	UINT hStrides = sizeof(VertType);
	UINT hOffsets = 0;

	view->hpDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &hStrides, &hOffsets);
	view->hpDeviceContext->VSSetConstantBuffers(1, 1, &transformBuffer);
	view->hpDeviceContext->IASetPrimitiveTopology(primitiveTopology);

	if (indexBuffer) {
		view->hpDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	}
}

template<class VertType>
void D3D11DrawElement<VertType>::Draw(const std::shared_ptr<D3DX11RenderView>& view) {
	this->SetBuffer(view);
	this->SetShader(view);

	if (indexBuffer) {
		view->hpDeviceContext->DrawIndexed(vertexCount, 0, 0);
	}
	else {
		view->hpDeviceContext->Draw(vertexCount, 0);
	}
}

template D3D11DrawElement<Vertex3D>;
template D3D11DrawElement<SimpleVertex>;
template D3D11DrawElement<PMDVertex>;
