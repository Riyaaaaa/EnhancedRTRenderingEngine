#pragma once
#include <d3d11.h>

#include "D3D11Texture.h"

class D3D11DepthStencilTarget
{
public:
	D3D11DepthStencilTarget();
	~D3D11DepthStencilTarget();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* hpDeviceContext);

	ID3D11DepthStencilView* GetDepthStencilView() const { return _depthStencilView; }
	ID3D11ShaderResourceView* const* GetSRV() const { return _texture.GetSubResourceViewRef(); }

protected:
	ID3D11Texture2D* _depthTexture;
	ID3D11DepthStencilView* _depthStencilView;
	D3D11Texture _texture;
};

