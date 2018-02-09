#pragma once
#include <d3d11.h>
#include "ComPtr.h"
#include "D3D11Texture.h"

class D3D11DepthStencilTarget
{
public:
    D3D11DepthStencilTarget();

    bool Initialize(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> hpDeviceContext);

    ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return _depthStencilView; }
    ComPtr<ID3D11ShaderResourceView> GetSRV() const { return _texture.GetSubResourceView(); }

protected:
    ComPtr<ID3D11Texture2D> _depthTexture;
    ComPtr<ID3D11DepthStencilView> _depthStencilView;
    D3D11Texture _texture;
};

