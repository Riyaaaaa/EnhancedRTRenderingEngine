#pragma once

#include <d3d11.h>

#include <memory>
#include <vector>

#include "../Structure/Structure.h"
#include "D3DX11Device.h"

#include "../Resource/ResourceHandle.h"
#include "../Scene/Scene.h"

class D3DX11Renderer
{
public:
	D3DX11Renderer();
	~D3DX11Renderer();

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext);

	void render(Scene* scene);
protected:
	std::unique_ptr<D3DX11RenderView> mView;
};

