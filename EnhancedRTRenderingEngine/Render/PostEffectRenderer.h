#pragma once

#include <memory>
#include <functional>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

class PostEffectRenderer
{
public:
    void Apply(GIImmediateCommands* cmd, GIRenderView* view, const std::string& effect);
};
