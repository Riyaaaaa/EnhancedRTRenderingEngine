#include "stdafx.h"
#include "HUDCanvasRenderer.h"

#include "Shader/ShaderFactory.h"

#include "nuklear/nuklear.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Common/Common.h"

static void
SetScreenProjection(int width, int height, Matrix* result)
{
    const float L = 0.0f;
    const float R = (float)width;
    const float T = 0.0f;
    const float B = (float)height;
    float matrix[4][4] =
    {
        { 2.0f / (R - L),              0.0f, 0.0f, 0.0f },
    { 0.0f,    2.0f / (T - B), 0.0f, 0.0f },
    { 0.0f,              0.0f, 0.5f, 0.0f },
    { (R + L) / (L - R), (T + B) / (B - T), 0.5f, 1.0f },
    };
    memcpy(result, matrix, sizeof(matrix));
}


HUDCanvasRenderer::HUDCanvasRenderer(GIImmediateCommands* cmd, GIRenderView* view, const NuklearWrapper& nuklear)
{
    std::vector<VertexLayout> layout;
    unsigned int offset = 0;
    layout.emplace_back("POSITION", VertexProperty::FloatRGBA, 0, 0, offsetof(Vertex2D, pos));
    layout.emplace_back("COLOR", VertexProperty::UnormRGBA, 0, 0, offsetof(Vertex2D, col));
    layout.emplace_back("TEXCOORD", VertexProperty::FloatRG, 0, 0, offsetof(Vertex2D, tex));

    _rstate = MakeRef(cmd->CreateRasterizerState(RasterizerType::HUD));

    BlendDesc bdesc;
    bdesc.AlphaToCoverageEnable = false;
    bdesc.RenderTarget[0].BlendEnable = TRUE;
    bdesc.RenderTarget[0].SrcBlend = BlendType::SRC_ALPHA;
    bdesc.RenderTarget[0].DestBlend = BlendType::INV_SRC_ALPHA;
    bdesc.RenderTarget[0].BlendOp = BlendTypeOp::ADD;
    bdesc.RenderTarget[0].SrcBlendAlpha = BlendType::INV_SRC_ALPHA;
    bdesc.RenderTarget[0].DestBlendAlpha = BlendType::ZERO;
    bdesc.RenderTarget[0].BlendOpAlpha = BlendTypeOp::ADD;
    bdesc.RenderTarget[0].RenderTargetWriteMask = ColorWriteEnable::COLOR_WRITE_ENABLE_ALL;
    _blendState = MakeRef(cmd->CreateBlendState(bdesc));

    {
        BufferDesc desc;
        desc.usage = ResourceUsage::Dynamic;
        desc.byteWidth = 512 * 1024;
        desc.accessFlag = ResourceAccessFlag::W;
        _vertexBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VertexList, desc));
    }

    {
        BufferDesc desc;
        desc.usage = ResourceUsage::Dynamic;
        desc.byteWidth = 128 * 1024;
        desc.accessFlag = ResourceAccessFlag::W;
        _indexBuffer = MakeRef(cmd->CreateBuffer(ResourceType::IndexList, desc));
    }

    {
        BufferDesc desc;
        desc.byteWidth = sizeof(_screenProjection);
        desc.usage = ResourceUsage::Dynamic;
        desc.accessFlag = ResourceAccessFlag::W;

        SetScreenProjection(view->GetRenderSize().w, view->GetRenderSize().h, &_screenProjection);
        _cbuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &_screenProjection));
    }

    {
        _vs = MakeRef(cmd->CreateVertexShader(ShaderFactory::HUDVertexShader()));
        _ps = MakeRef(cmd->CreatePixelShader(ShaderFactory::HUDPixelShader()));
    }

    {
        SamplerParam param;
        param.addressMode = TextureAddressMode::CLAMP;
        _samplerState = MakeRef(cmd->CreateSamplerState(param));
    }

    _layout = MakeRef(cmd->CreateInputLayout(layout, _vs.get()));

}

HUDCanvasRenderer::~HUDCanvasRenderer() 
{
}

void HUDCanvasRenderer::update(GIImmediateCommands* cmd, GIRenderView* view, HUDCanvas* canvas, const NuklearWrapper& nuklear) {
    auto* ctx = nuklear.Context();

    struct nk_colorf bg;
    bg.r = 0.10f, bg.g = 0.18f, bg.b = 0.24f, bg.a = 1.0f;

    // TODO: create layout from HUDCanvas
    if (nk_begin(ctx, "Demo", nk_rect(50, 50, 400, 500),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
        NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE))
    {
        enum { EASY, HARD };
        static int op = EASY;
        static int property = 20;

        nk_layout_row_static(ctx, 30, 80, 1);
        if (nk_button_label(ctx, "button"))
            fprintf(stdout, "button pressed\n");
        nk_layout_row_dynamic(ctx, 60, 2);
        if (nk_option_label(ctx, "easy", op == EASY)) op = EASY;
        if (nk_option_label(ctx, "hard", op == HARD)) op = HARD;
        nk_layout_row_dynamic(ctx, 50, 1);
        nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "background:", NK_TEXT_LEFT);
        nk_layout_row_dynamic(ctx, 25, 1);
        if (nk_combo_begin_color(ctx, nk_rgb_cf(bg), nk_vec2(nk_widget_width(ctx), 400))) {
            nk_layout_row_dynamic(ctx, 120, 1);
            bg = nk_color_picker(ctx, bg, NK_RGBA);
            nk_layout_row_dynamic(ctx, 25, 1);
            bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f, 0.005f);
            bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f, 0.005f);
            bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f, 0.005f);
            bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f, 0.005f);
            nk_combo_end(ctx);
        }
    }
    nk_end(ctx);
}

void HUDCanvasRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, HUDCanvas* canvas, const NuklearWrapper& nuklear) {
    const Vector4D blend_factor(0.0f, 0.0f, 0.0f, 0.0f);

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    cmd->IASetInputLayout(_layout.get());
    cmd->IASetVertexBuffer(_vertexBuffer.get(), sizeof(Vertex2D), 0);
    cmd->IASetIndexBuffer(_indexBuffer.get(), 0);
    cmd->IASetPrimitiveTopology(VertexPrimitiveType::TRIANGLELIST);
    cmd->VSSetShader(_vs.get());
    cmd->VSSetConstantBuffers(0, _cbuffer.get());
    cmd->PSSetShader(_ps.get());
    cmd->PSSetSamplers(0, _samplerState.get());
    cmd->OMSetBlendState(_blendState.get(), blend_factor);
    cmd->RSSetState(_rstate.get());

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    auto mapped_vertex_buffer = cmd->MapBuffer(_vertexBuffer.get(), 0, MapType::WRITE_DISCARD);
    auto mapped_index_buffer = cmd->MapBuffer(_indexBuffer.get(), 0, MapType::WRITE_DISCARD);

    {
        struct nk_convert_config config;
        NK_STORAGE const struct nk_draw_vertex_layout_element vertex_layout[] = {
            { NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(Vertex2D, pos) },
            { NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(Vertex2D, tex) },
            { NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(Vertex2D, col) },
        { NK_VERTEX_LAYOUT_END }
        };
        memset(&config, 0, sizeof(config));
        config.vertex_layout = vertex_layout;
        config.vertex_size = sizeof(Vertex2D);
        config.vertex_alignment = NK_ALIGNOF(Vertex2D);
        config.global_alpha = 1.0f;
        config.shape_AA = NK_ANTI_ALIASING_ON;
        config.line_AA = NK_ANTI_ALIASING_ON;
        config.circle_segment_count = 22;
        config.curve_segment_count = 22;
        config.arc_segment_count = 22;
        config.null = *nuklear.Nulltexture();

        {/* setup buffers to load vertices and elements */
            struct nk_buffer vbuf, ibuf;
            nk_buffer_init_fixed(&vbuf, mapped_vertex_buffer.pData, (size_t)MAX_VERTEX_BUFFER);
            nk_buffer_init_fixed(&ibuf, mapped_index_buffer.pData, (size_t)MAX_INDEX_BUFFER);
            nk_convert(nuklear.Context(), nuklear.Commands(), &vbuf, &ibuf, &config); 

            /*Vertex2D* vertPtr = (Vertex2D*)vbuf.memory.ptr;
            int count = 0;
            for (int i = 0; i < vbuf.memory.size / sizeof(Vertex2D); i++) {
                auto ptr = vertPtr + i;

                Vector4D v(ptr->pos.x, ptr->pos.y, 0, 0);
                Vector4D projected = v * _screenProjection;

                if (projected.x == 0 && projected.y == 0) 
                {
                    break;
                }

                ERTREDebug(L"%f %f\n", projected.x, projected.y);
                count++;
            }
            ERTREDebug(L"nums: %d\n", count);


            unsigned short* idxPtr = (unsigned short*)ibuf.memory.ptr;
            for (int i = 0; i < ibuf.memory.size / sizeof(Vertex2D) - 1; i++) {
                auto ptr = idxPtr + i;
                if (*ptr == 0 && *(ptr + 1) == 0) {
                    break;
                }
                ERTREDebug(L"%u, ", *ptr);
            }*/
        }
    }

    cmd->UnmapBuffer(_vertexBuffer.get(), 0);
    cmd->UnmapBuffer(_indexBuffer.get(), 0);

    const struct nk_draw_command *nkcmd;
    UINT offset = 0;
    nk_draw_foreach(nkcmd, nuklear.Context(), nuklear.Commands())
    {
        ScissorRect scissor;
        GIShaderResourceView *texture_view = (GIShaderResourceView*)nkcmd->texture.ptr;
        if (!nkcmd->elem_count) continue;

        scissor.left = (LONG)nkcmd->clip_rect.x;
        scissor.right = (LONG)(nkcmd->clip_rect.x + nkcmd->clip_rect.w);
        scissor.top = (LONG)nkcmd->clip_rect.y;
        scissor.bottom = (LONG)(nkcmd->clip_rect.y + nkcmd->clip_rect.h);

        cmd->PSSetShaderResources(0, texture_view);
        cmd->RSSetScissorRect(scissor);
        cmd->DrawIndexed((UINT)nkcmd->elem_count, offset, 0);
        offset += nkcmd->elem_count;
    }
    nk_clear(nuklear.Context());
}
