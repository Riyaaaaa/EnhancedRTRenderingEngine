#pragma once

#include "GUI/HUDCanvas.h"
#include "GUI/NuklearWrapper.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GIRenderView.h"

struct nk_context;
struct nk_font_atlas;
struct nk_buffer;

class HUDCanvasRenderer
{
public:
    HUDCanvasRenderer(GIImmediateCommands* cmd, GIRenderView* view, const std::shared_ptr<NuklearWrapper>& nuklear);
    ~HUDCanvasRenderer();

    void update(GIImmediateCommands* cmd, HUDCanvas* canvas, const std::shared_ptr<NuklearWrapper>& nuklear);
    void render(GIImmediateCommands* cmd, GIRenderView* view, const std::shared_ptr<NuklearWrapper>& nuklear);

protected:
    static constexpr unsigned int MAX_VERTEX_BUFFER = 512 * 1024;
    static constexpr unsigned int MAX_INDEX_BUFFER = 128 * 1024;

    std::shared_ptr<GIPixelShader> _ps;
    std::shared_ptr<GIVertexShader> _vs;
    std::shared_ptr<GIBuffer> _vertexBuffer, _indexBuffer, _cbuffer;
    std::shared_ptr<GIBlendState> _blendState;
    std::shared_ptr<GIRasterizerState> _rstate;
    std::shared_ptr<GISamplerState> _samplerState;
    std::shared_ptr<GIInputLayout> _layout;

    Matrix _screenProjection;
};



