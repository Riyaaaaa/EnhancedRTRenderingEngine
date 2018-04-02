#pragma once

#include <memory>
#include <functional>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class D3D11PostEffectRenderer
{
public:
    void Apply(GIImmediateCommands* cmd, GIRenderView* view, const std::string& effect);
};
