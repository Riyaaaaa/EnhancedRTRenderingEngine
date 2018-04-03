#pragma once

#include <d3d11.h>

#include <memory>
#include <functional>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

std::shared_ptr<GITexture2D> D3D11GaussianFilter(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> src);

