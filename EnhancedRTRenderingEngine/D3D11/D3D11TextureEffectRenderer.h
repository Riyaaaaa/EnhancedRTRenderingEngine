#pragma once

#include <d3d11.h>

#include <memory>

#include "D3D11DepthStencilTarget.h"
#include "D3DX11RenderView.h"
#include "D3D11SceneInfo.h"

class D3D11TextureEffectRenderer
{
public:
    bool Initialize(const std::shared_ptr<D3DX11RenderView>& view);
    D3D11Texture apply(const D3D11Texture& src, const std::string& effect);
protected:
    std::shared_ptr<D3DX11RenderView> _view;
};

