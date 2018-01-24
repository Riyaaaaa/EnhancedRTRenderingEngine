#pragma once

#include <d3d11.h>

#include <memory>
#include <vector>

#include "../Structure/Structure.h"
#include "D3DX11Device.h"

#include "../Resource/ResourceHandle.h"

class D3DX11Renderer
{
public:
	D3DX11Renderer();
	~D3DX11Renderer();

	void Initialize(const std::shared_ptr<D3DX11Device>& device, const std::array<Vertex3D, 3>& vData );

	void render();
protected:
	std::shared_ptr<D3DX11Device> mDevice;

	std::vector<ResourceHandle> mResourceQueue;
};

