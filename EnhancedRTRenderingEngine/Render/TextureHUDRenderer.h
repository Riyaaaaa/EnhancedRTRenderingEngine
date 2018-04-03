#pragma once

#include <memory>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"
#include "GraphicsInterface/GITextureProxy.h"

#include "Structure/Structure.h"

class D3D11TextureHUDRenderer
{
public:
    void render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size size, const Texture2D& texture);
    void render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size size, const GITextureProxy& texture, int index = 0);
};
