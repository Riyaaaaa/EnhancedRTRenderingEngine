#pragma once

#include <memory>
#include <functional>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

std::shared_ptr<GITexture2D> GaussianFilter(GIImmediateCommands* cmd, std::shared_ptr<GITexture2D> src);

