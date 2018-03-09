#pragma once

#include <d3d11.h>

#include <memory>

#include "D3D11DepthStencilTarget.h"
#include "D3DX11RenderView.h"
#include "D3D11Scene.h"

class D3D11TextureHUDRenderer
{
public:
    bool Initialize(const std::shared_ptr<D3DX11RenderView>& view);
    void render(Vector2D pos, Size size, const Texture2D& texture);
    void render(Vector2D pos, Size size, const D3D11Texture& texture, int index);
protected:
    std::shared_ptr<D3DX11RenderView> _view;
};
