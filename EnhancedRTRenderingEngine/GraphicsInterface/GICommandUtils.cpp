#include "stdafx.h"
#include "GICommandUtils.h"

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

}