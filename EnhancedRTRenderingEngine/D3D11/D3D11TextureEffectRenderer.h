#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3D11ConstantBufferBuilder.h"
#include "D3D11OMResource.h"
#include "D3DX11RenderView.h"
#include "D3D11SceneInfo.h"

class D3D11TextureEffectRenderer
{
public:
    bool Initialize(const std::shared_ptr<D3DX11RenderView>& view);

    template<class BufferType>
    void SetConstantBuffer(BufferType* buffer) {
        _constantBuffer = D3D11ConstantBufferBuilder::BuildConstantBuffer<BufferType>(_view->hpDevice, buffer);
    }

    D3D11TextureProxy Apply(const D3D11TextureProxy& src, const std::string& effect);
protected:
    std::shared_ptr<D3DX11RenderView> _view;
    ComPtr<ID3D11Buffer> _constantBuffer;
};

