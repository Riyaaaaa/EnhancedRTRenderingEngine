#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3DX11Device.h"
#include "D3D11Texture.h"
#include "Scene/MeshObject.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"

template<class VertType>
class D3D11DrawElement
{
public:
	D3D11DrawElement(ID3D11Device* device, MeshObject<VertType>* element);
	~D3D11DrawElement();

	void Draw(const std::unique_ptr<D3DX11RenderView>& device);

protected:
	bool SetBuffer(ID3D11Device* device, MeshObject<VertType>* element);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

	D3D11Texture tex;
	SIZE_T vertexCount;

	ID3D11Buffer* transformBuffer;
	ID3D11Buffer* vertexBuffer;

	ResourceHandle<> vShader, pShader;

	RenderingState _state;
};

