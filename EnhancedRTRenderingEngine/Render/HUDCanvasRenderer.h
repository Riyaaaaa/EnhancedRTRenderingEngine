#pragma once

#include "GUI/HUDCanvas.h"
#include "GUI/NuklearContexts.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

struct nk_context;
struct nk_font_atlas;
struct nk_buffer;

class HUDCanvasRenderer
{
public:
    HUDCanvasRenderer(GIImmediateCommands* cmd, GIRenderView* view, const NuklearContexts& contexts);
    ~HUDCanvasRenderer();

    void render(GIImmediateCommands* cmd, GIRenderView* view, HUDCanvas* canvas);

protected:
};



