#pragma once

namespace RenderTag {
    struct OpaqueRender {};
    struct DepthRender {};
}

#define OpaqueRenderTag RenderTag::OpaqueRender{}
#define DepthRenderTag RenderTag::DepthRender{}
