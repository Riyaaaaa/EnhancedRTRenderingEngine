#pragma once

#include <d3d11.h>

#include "Resource/Texture2D.h"

class D3D11Texture
{
	ID3D11Texture2D* mTexture;
	ID3D11ShaderResourceView* mView;
	ID3D11SamplerState* mSampler;

public:
	bool IsAvalable() {
		return mTexture != nullptr && mView != nullptr && mSampler != nullptr;
	}
	bool Initialize(ID3D11Device* device, const ResourceHandle<Texture2D>& tex);

	ID3D11ShaderResourceView* GetSubResourceView() const
	{
		return mView;
	}

	ID3D11ShaderResourceView* const* GetSubResourceViewRef() const
	{
		return &mView;
	}

	ID3D11SamplerState* const* GetSamplerRef()
	{
		return &mSampler;
	}

	~D3D11Texture();
};
