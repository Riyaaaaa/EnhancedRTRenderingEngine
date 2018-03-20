#pragma once
#include <d3d11.h>
#include "ComPtr.h"
#include "D3D11TextureProxy.h"

class D3D11OMResource
{
public:
    D3D11OMResource(const ComPtr<ID3D11Device>& device, Size size);

    bool InitializeRenderTarget(ComPtr<ID3D11DeviceContext> hpDeviceContext, bool useAsShaderResource);
    bool InitializeDepthStencilView(ComPtr<ID3D11DeviceContext> hpDeviceContext, bool useAsShaderResource);

    ID3D11RenderTargetView** GetRenderTargetRef() const { return _renderTarget.Ref(); }
    ID3D11RenderTargetView* GetRenderTarget() const { return _renderTarget.Get(); }
    ID3D11DepthStencilView* GetDepthStencilView() const { return _depthStencilView.Get(); }

    ID3D11ShaderResourceView* GetRTVSRV() const { return _RTVTexture->GetSubResourceView().Get(); }
    ID3D11SamplerState* GetRTVSampler() const { return _RTVTexture->GetSampler().Get(); }

    ID3D11ShaderResourceView* GetDSVSRV() const { return _DSVTexture->GetSubResourceView().Get(); }
    ID3D11SamplerState* GetDSVSampler() const { return _DSVTexture->GetSampler().Get(); }

    const D3D11TextureProxy& GetRTVTexture() const { return _RTVTexture; }
    const D3D11TextureProxy& GetDSVTexture() const { return _DSVTexture; }

protected:
    ComPtr<ID3D11RenderTargetView> _renderTarget;
    ComPtr<ID3D11DepthStencilView> _depthStencilView;
    ComPtr<ID3D11Device> _device;

    D3D11TextureProxy _RTVTexture;
    D3D11TextureProxy _DSVTexture;

    Size _size;
};

