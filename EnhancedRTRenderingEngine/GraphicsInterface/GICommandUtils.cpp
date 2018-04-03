#include "stdafx.h"
#include "GICommandUtils.h"

#include "Utility/TextureUtils.h"

std::shared_ptr<GIRenderTargetView> GICommandUtils::CreateRenderTargetView(GIImmediateCommands* cmd, Size size, TextureFormat format, bool useAsResource) {
    TextureParam param;
    param.width = size.w;
    param.height = size.h;
    param.format = format;
    param.bindFlag = useAsResource ? TextureBindTarget::SHADER_RESOURCE | TextureBindTarget::RENDER_TARGET : TextureBindTarget::RENDER_TARGET;

    auto tex = MakeRef(cmd->CreateTexture2D(param));
    auto rtv = MakeRef(cmd->CreateRenderTargetView(tex.get()));
    rtv->rtvTexture = tex;

    return rtv;
}

std::shared_ptr<GIDepthStencilView> GICommandUtils::CreateDepthStencilView(GIImmediateCommands* cmd, Size size, TextureFormat format, bool useAsResource) {
    TextureParam param;
    param.width = size.w;
    param.height = size.h;
    param.format = TextureUtils::ZBufferFormatToTextureFormat(format);
    param.bindFlag = useAsResource ? TextureBindTarget::SHADER_RESOURCE | TextureBindTarget::STENCIL : TextureBindTarget::STENCIL;

    auto tex = MakeRef(cmd->CreateTexture2D(param));
    auto dsv = MakeRef(cmd->CreateDepthStencilView(tex.get(), format));
    dsv->dsvTexture = tex;

    return dsv;
}

void GICommandUtils::SetViewportSize(GIImmediateCommands* cmd, Size size) {
    ViewportCfg cfg;

    cfg.height = size.h;
    cfg.width = size.w;
    cfg.maxDepth = 1.0f;
    cfg.minDepth = 0.0f;
    cfg.topLeftX = 0;
    cfg.topLeftY = 0;

    cmd->SetViewport(cfg);
}
