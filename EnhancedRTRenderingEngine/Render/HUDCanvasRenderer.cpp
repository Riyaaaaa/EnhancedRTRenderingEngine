#include "stdafx.h"
#include "HUDCanvasRenderer.h"

HUDCanvasRenderer::HUDCanvasRenderer(GIImmediateCommands* cmd, GIRenderView* view, const NuklearContexts& contexts)
{
    std::vector<VertexLayout> layout;
    unsigned int offset = 0;
    layout.emplace_back("POSITION", VertexProperty::FloatRGBA, 0, 0, 0);
    offset += GetMemoryBlockSize(layout.back().vProperty);
    layout.emplace_back("TEXCOORD", VertexProperty::FloatRG, 0, 0, offset);
    offset += GetMemoryBlockSize(layout.back().vProperty);
    layout.emplace_back("POSITION", VertexProperty::UnormRGBA, 0, 0, offset);


//    cmd->CreateInputLayout(layout);
}

HUDCanvasRenderer::~HUDCanvasRenderer() 
{
}

void HUDCanvasRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, HUDCanvas* canvas) {

}
