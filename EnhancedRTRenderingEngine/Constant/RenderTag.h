#pragma once

namespace RenderTag {
    struct OpaqueRender {};
    struct DepthRender {};
    struct HUDRender {};
}

#define OpaqueRenderTag RenderTag::OpaqueRender{}
#define DepthRenderTag RenderTag::DepthRender{}
#define HUDRenderTag RenderTag::HUDRender{}
