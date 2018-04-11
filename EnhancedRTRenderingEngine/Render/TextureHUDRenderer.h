#pragma once

#include <memory>

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"
#include "GraphicsInterface/GITextureProxy.h"

#include "Structure/Structure.h"

class TextureHUDRenderer
{
public:
    static void render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size2D size, const Texture2D& texture);
    static void render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size2D size, const GITextureProxy& texture, int index = 0);
};
