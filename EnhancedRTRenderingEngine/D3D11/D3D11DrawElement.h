#pragma once

#include <d3d11.h>
#include <vector>
#include <memory>

#include "D3DX11RenderView.h"
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

	virtual void Draw(const std::shared_ptr<D3DX11RenderView>& view);

protected:
	virtual bool CreateBuffer(ID3D11Device* device, MeshObject<VertType>* element);
	
	virtual void SetBuffer(const std::shared_ptr<D3DX11RenderView>& view);
	virtual void SetShader(const std::shared_ptr<D3DX11RenderView>& view);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inElemDesc;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology;

	D3D11Texture tex;
	SIZE_T vertexCount;

	ID3D11Buffer* transformBuffer;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	ResourceHandle<> vShader, pShader;

	RenderingState _state;
};

