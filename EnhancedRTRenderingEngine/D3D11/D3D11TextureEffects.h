#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "D3D11RenderView.h"

D3D11TextureProxy D3D11GaussianFilter(const std::shared_ptr<D3D11RenderView>& view, const D3D11TextureProxy& src);

