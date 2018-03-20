#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3D11ConstantBufferBuilder.h"
#include "D3D11OMResource.h"
#include "D3D11RenderView.h"
#include "D3D11SceneInfo.h"

class D3D11PostEffectRenderer
{
public:
    bool Initialize(const std::shared_ptr<D3D11RenderView>& view);
    void Apply(const std::string& effect);
protected:
    std::shared_ptr<D3D11RenderView> _view;
};
