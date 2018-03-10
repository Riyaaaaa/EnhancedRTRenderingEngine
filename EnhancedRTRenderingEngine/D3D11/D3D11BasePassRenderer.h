#pragma once

#include <d3d11.h>

#include <memory>

#include "D3D11Texture.h"
#include "D3DX11RenderView.h"
#include "D3D11SceneInfo.h"

class D3D11BasePassRenderer
{
public:
    D3D11BasePassRenderer();
    ~D3D11BasePassRenderer();

    void render(D3D11SceneInfo* scene);
    bool Initialize(const std::shared_ptr<D3DX11RenderView>& view);

protected:
    std::shared_ptr<D3DX11RenderView> _view;
};

