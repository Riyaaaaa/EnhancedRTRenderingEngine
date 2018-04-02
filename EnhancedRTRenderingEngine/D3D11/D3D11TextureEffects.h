#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

GITextureProxy D3D11GaussianFilter(GIImmediateCommands* cmd, GIRenderView* view, std::shared_ptr<GITexture2D> src);

