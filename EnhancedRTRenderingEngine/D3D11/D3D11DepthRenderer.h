#pragma once

#include <d3d11.h>

#include <memory>

#include "D3DX11RenderView.h"
#include "../Scene/Scene.h"

class D3D11DepthRenderer
{
public:
	bool Initialize(const std::shared_ptr<D3DX11RenderView>& view);
	void render(Scene* scene);


protected:
	ID3D11DepthStencilView * lightDepthView;
	std::shared_ptr<D3DX11RenderView> _view;
};

