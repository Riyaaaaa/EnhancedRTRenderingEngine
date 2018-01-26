#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3DX11Device.h"
#include "D3D11Texture.h"
#include "Mesh/MeshBase.h"
#include "Resource/ResourceHandle.h"
#include "RenderingContext.h"

template<class VertType>
class D3D11DrawElement
{
public:
	D3D11DrawElement(ID3D11Device* device, RenderingContext context, MeshBase<VertType>* mesh);
	void Draw(const std::unique_ptr<D3DX11RenderView>& device);

protected:
	std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subResource;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

	D3D11Texture tex;
	SIZE_T vertexCount;

	ResourceHandle vShader, pShader;
};

