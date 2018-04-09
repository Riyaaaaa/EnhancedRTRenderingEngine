#include "stdafx.h"
#include "GIRenderView.h"

#include "GICommandUtils.h"

void GIRenderView::Initialize(GIImmediateCommands* cmd, const ViewportParam& param) {
    _swapchain = MakeRef(cmd->CreateSwapChain(param));

    _OMResource = std::make_shared<GIOMResource>();
    _OMResource->renderTargets.push_back(MakeRef(cmd->CreateRenderTargetView(_swapchain->backBuffer.get())));
    _OMResource->depthStencilView = GICommandUtils::CreateDepthStencilView(cmd, Size(param.cfg.width, param.cfg.height), TextureFormat::D24_UNORM_S8_UINT, false);

    _defaultRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::Default));
    _doubleSidedRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::DoubleSided));
    _wireframeRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::WireFrame));

    cmd->RSSetState(_defaultRState.get());

    _type = param.MSAAQuality;
    _renderSize = Size(param.cfg.width, param.cfg.height);
    _viewportCfg = param.cfg;
    cmd->SetViewport(_viewportCfg);
}

void GIRenderView::SetRasterizerState(GIImmediateCommands* cmd, RasterizerState state) {
    switch (state) {
    case RasterizerState::Default:
        cmd->RSSetState(_defaultRState.get());
        break;
    case RasterizerState::CullNone:
        cmd->RSSetState(_doubleSidedRState.get());
        break;
    case RasterizerState::WireFrame:
        cmd->RSSetState(_wireframeRState.get());
        break;
    }
}

void GIRenderView::SetViewPortSize(GIImmediateCommands* cmd, Size size) {
    _viewportCfg.width = size.w;
    _viewportCfg.height = size.h;
    cmd->SetViewport(_viewportCfg);
}

void GIRenderView::Present() {
    _swapchain->Present(0, 0);
}

