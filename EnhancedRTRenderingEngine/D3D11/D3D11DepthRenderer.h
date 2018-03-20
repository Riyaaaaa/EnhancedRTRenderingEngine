#pragma once

#include <d3d11.h>

#include <memory>

#include "D3D11OMResource.h"
#include "D3D11RenderView.h"
#include "D3D11SceneInfo.h"

class D3D11DepthRenderer
{
public:
    bool Initialize(const std::shared_ptr<D3D11RenderView>& view);
    void render(D3D11SceneInfo* scene);

protected:
    void RenderDirectionalLightShadowMap(D3D11SceneInfo* scene);
    void RenderPointLightShadowMap(D3D11SceneInfo* scene);

    std::shared_ptr<D3D11RenderView> _view;
};

