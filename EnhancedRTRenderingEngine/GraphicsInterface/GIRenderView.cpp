#include "stdafx.h"
#include "GIRenderView.h"


void GIRenderView::SetRasterizerState(GIImmediateCommands* cmd, RasterizerState state) {
    switch (state) {
    case RasterizerState::Default:
        cmd->RSSetState(_defaultRState);
        break;
    case RasterizerState::CullNone:
        cmd->RSSetState(_doubleSidedRState);
        break;
    case RasterizerState::WireFrame:
        cmd->RSSetState(_wireframeRState);
        break;
    }
}

void GIRenderView::SetViewPortSize(GIImmediateCommands* cmd, Size size) {
    cmd->RSSetViewports(size);
}

