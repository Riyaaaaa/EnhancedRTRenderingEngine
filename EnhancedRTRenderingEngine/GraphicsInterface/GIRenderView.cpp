#include "stdafx.h"
#include "GIRenderView.h"


void GIRenderView::Initialize(GIImmediateCommands* cmd, const ViewportParam& param) {
    _swapchain = MakeRef(cmd->CreateSwapChain(param));

    _OMResource = std::make_shared<GIOMResource>();
    _OMResource->renderTargets.push_back(MakeRef(cmd->CreateRenderTargetView(_swapchain->backBuffer.get())));

    TextureParam dsvTexParam;
    dsvTexParam.width = param.cfg.width;
    dsvTexParam.height = param.cfg.height;
    dsvTexParam.bindFlag = TextureBindTarget::STENCIL | TextureBindTarget::SHADER_RESOURCE;
    dsvTexParam.format = TextureFormat::R24G8_TYPELESS;

    _dsvTexture = MakeRef(cmd->CreateTexture2D(dsvTexParam, std::vector<Texture2D>()));
    _OMResource->depthStencilView = MakeRef(cmd->CreateDepthStencilView(_dsvTexture.get()));

    _defaultRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::Defalt));
    _doubleSidedRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::DoubleSided));
    _wireframeRState = MakeRef(cmd->CreateRasterizerState(RasterizerType::WireFrame));

    cmd->RSSetState(_defaultRState.get());

    _type = param.MSAAQuality;
    _viewportCfg = param.cfg;
    cmd->SetViewPortSize(_viewportCfg);
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
    cmd->SetViewPortSize(_viewportCfg);
}

