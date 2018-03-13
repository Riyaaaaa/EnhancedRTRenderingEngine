#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3DX11RenderView.h"

D3D11TextureProxy D3D11GaussianFilter(const std::shared_ptr<D3DX11RenderView>& view, D3D11TextureProxy& src);

