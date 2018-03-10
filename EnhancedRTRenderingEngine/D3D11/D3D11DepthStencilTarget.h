#pragma once
#include <d3d11.h>
#include "ComPtr.h"
#include "D3D11Texture.h"

class D3D11DepthStencilTarget
{
public:
    D3D11DepthStencilTarget(const ComPtr<ID3D11Device>& device);

    bool Initialize(ComPtr<ID3D11DeviceContext> hpDeviceContext, Size size);

    ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return _depthStencilView; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return _texture.GetSubResourceView(); }
    ComPtr<ID3D11SamplerState> GetSampler() const { return _texture.GetSampler(); }
    const D3D11Texture& GetTexture() const { return _texture; }


protected:
    ComPtr<ID3D11DepthStencilView> _depthStencilView;
    ComPtr<ID3D11Device> _device;
    D3D11Texture _texture;
};

