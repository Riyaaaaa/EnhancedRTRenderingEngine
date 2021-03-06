#pragma once

#include "GIImmediateCommands.h"

namespace GICommandUtils
{
    std::shared_ptr<GIRenderTargetView> CreateRenderTargetView(GIImmediateCommands* cmd, Size2Dd size, TextureFormat format, bool useAsResource);
    std::shared_ptr<GIDepthStencilView> CreateDepthStencilView(GIImmediateCommands* cmd, Size2Dd size, TextureFormat format, bool useAsResource);

    void SetViewportSize(GIImmediateCommands* cmd, Size2Dd size);
}

