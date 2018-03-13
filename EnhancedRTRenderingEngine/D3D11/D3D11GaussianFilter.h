#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3D11TextureEffectRenderer.h"
#include "D3DX11RenderView.h"
#include "D3D11SceneInfo.h"

D3D11Texture D3D11GaussianFilter(const std::shared_ptr<D3DX11RenderView>& view, D3D11Texture& src);

