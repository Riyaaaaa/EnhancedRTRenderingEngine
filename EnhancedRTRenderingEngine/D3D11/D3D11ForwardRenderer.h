#pragma once

#include <d3d11.h>

#include <memory>
#include <vector>

#include "../Structure/Structure.h"
#include "D3DX11RenderView.h"
#include "../Scene/Scene.h"

#include "D3D11DepthRenderer.h"
#include "D3D11BasePassRenderer.h"

class D3D11ForwardRenderer
{
public:

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext);

	D3D11ForwardRenderer();
	~D3D11ForwardRenderer();

	void render(Scene* scene);

protected:
	std::shared_ptr<D3DX11RenderView> mView;

	D3D11DepthRenderer depthRenderer;
	D3D11BasePassRenderer bassPassRenderer;
};

