

#include <memory>
#include <cstdint>

#include "HUDCanvasRenderer.h"

#include "Shader/ShaderFactory.h"

#include "nuklear/nuklear.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "GUI/NuklearFormatUtils.h"
#include "GUI/UIWidgets.h"

#include "UserData/UserConfig.h"

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


HUDCanvasRenderer::HUDCanvasRenderer(GIImmediateCommands* cmd, GIRenderView* view, const std::shared_ptr<NuklearWrapper>& nuklear)
{
    std::vector<VertexLayout> layout;
    unsigned int offset = 0;
    layout.emplace_back("POSITION", VertexProperty::FloatRGBA, 0, 0, static_cast<unsigned int>(offsetof(Vertex2D, pos)));
    layout.emplace_back("COLOR", VertexProperty::UnormRGBA, 0, 0, static_cast<unsigned int>(offsetof(Vertex2D, col)));
    layout.emplace_back("TEXCOORD", VertexProperty::FloatRG, 0, 0, static_cast<unsigned int>(offsetof(Vertex2D, tex)));

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

    _nuklear = nuklear;
}

HUDCanvasRenderer::~HUDCanvasRenderer() 
{
}

static void GenerateNuklearLayout(const UIRowLayout& row, const std::shared_ptr<NuklearWrapper>& nuklear);
static void GenerateNuklearLayout(const std::shared_ptr<UIWidget>& widget, const std::shared_ptr<NuklearWrapper>& nuklear);
static void GenerateNuklearLayout(HUDCanvas* canvas, const std::shared_ptr<NuklearWrapper>& nuklear);

static void GenerateNuklearLayout(const std::shared_ptr<UIWidget>& widget, const std::shared_ptr<NuklearWrapper>& nuklear) {
    auto* ctx = nuklear->Context();

    switch (widget->WidgetType()) {
    case UIWidgetType::ButtonLabel: {
        auto buttonLabel = std::static_pointer_cast<UIButtonLabel>(widget);
        if (nk_button_label(ctx, buttonLabel->Text().c_str())) {
            buttonLabel->CallHandler();
        }
            
    }
        break;
    case UIWidgetType::Label: {
        auto label = std::static_pointer_cast<UILabel>(widget);
        nk_label(ctx, label->Text().c_str(), CastToNuklearFormat<nk_text_alignment>(label->AlignType()));
    }
                              break;
    case UIWidgetType::IntProperty: {
        auto int_property = std::static_pointer_cast<UINumericProperty<int>>(widget);
        nk_property_int(ctx, 
            int_property->Text().c_str(), 
            int_property->Min(), 
            int_property->PropertyRef(), 
            int_property->Max(), 
            int_property->Step(), 
            int_property->PerPixel());
    }
                                    break;
    case UIWidgetType::FloatProperty: {
        auto float_property = std::static_pointer_cast<UINumericProperty<float>>(widget);
        nk_property_float(ctx,
            float_property->Text().c_str(),
            float_property->Min(),
            float_property->PropertyRef(),
            float_property->Max(),
            float_property->Step(),
            float_property->PerPixel());
    }
                                    break;
    case UIWidgetType::RadioButton: {
        auto option_label = std::static_pointer_cast<UIRadioButton>(widget);
        if (nk_option_label(ctx, option_label->Text().c_str(), option_label->Condition())) {
            option_label->CallHandler();
        }
    }
                                    break;
    case UIWidgetType::ColorPicker: {
        auto color_picker = std::static_pointer_cast<UIColorPicker>(widget);
        auto* color = color_picker->Color();

        if (color_picker->UseAlpha()) {
            nk_color_pick(ctx, reinterpret_cast<nk_colorf*>(color), NK_RGBA);
        }
        else {
            nk_color_pick(ctx, reinterpret_cast<nk_colorf*>(color), NK_RGB);
        }
    }
                                    break;
    case UIWidgetType::ComboColor:
        auto combo_color = std::static_pointer_cast<UIComboColor>(widget);
        nk_color col = nk_rgb_f(combo_color->Color().x, combo_color->Color().y, combo_color->Color().z);

        float height, width;

        height = combo_color->Size().h;
        if (combo_color->IsFitting()) {
            width = nk_widget_width(ctx);
        }
        else {
            width = combo_color->Size().h;
        }

        if (nk_combo_begin_color(ctx, col, nk_vec2(width, height))) {

            for (auto& row : combo_color->ContainWidgets()) {
                GenerateNuklearLayout(row, nuklear);
            }

            nk_combo_end(ctx);
        }
    }
}

static void GenerateNuklearLayout(const UIRowLayout& row, const std::shared_ptr<NuklearWrapper>& nuklear) {
    auto* ctx = nuklear->Context();

    switch (row.Attribute()) {
    case LayoutAttribute::Dynamic:
        nk_layout_row_dynamic(ctx, row.Height(), row.Cols());
        break;
    case LayoutAttribute::Static:
        nk_layout_row_static(ctx, row.Height(), row.ItemWidth(), row.Cols());
        break;
    }

    for (auto&& widget: row.Widgets()) {
        GenerateNuklearLayout(widget, nuklear);
    }
}

static void GenerateNuklearLayout(HUDCanvas* canvas, const std::shared_ptr<NuklearWrapper>& nuklear) {
    for (auto&& window : canvas->Windows()) {
        if (nk_begin(nuklear->Context(), window.Title().c_str(), nk_rect(50, 50, window.Size().w, window.Size().h),
            NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
            NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
            for (auto&& row : window.Rows()) {
                GenerateNuklearLayout(row, nuklear);
            }

            BoundingBox2D box;
            auto rect = nk_window_get_bounds(nuklear->Context());

            box.pos.x = rect.x;
            box.pos.y = rect.y;
            box.size.w = rect.w;
            box.size.h = rect.h;

            nuklear->AddWindowRects(box);
        }

        nk_end(nuklear->Context());
    }
}

void HUDCanvasRenderer::update(GIImmediateCommands* cmd, HUDCanvas* canvas, const std::shared_ptr<NuklearWrapper>& nuklear) {
    auto* ctx = _nuklear->Context();
    nuklear->ClearWindowRects();
    GenerateNuklearLayout(canvas, nuklear);
}

void HUDCanvasRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, const std::shared_ptr<NuklearWrapper>& nuklear) {
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
        config.null = *nuklear->Nulltexture();

        {/* setup buffers to load vertices and elements */
            struct nk_buffer vbuf, ibuf;
            nk_buffer_init_fixed(&vbuf, mapped_vertex_buffer.pData, (size_t)MAX_VERTEX_BUFFER);
            nk_buffer_init_fixed(&ibuf, mapped_index_buffer.pData, (size_t)MAX_INDEX_BUFFER);
            nk_convert(nuklear->Context(), nuklear->Commands(), &vbuf, &ibuf, &config); 

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
    nk_draw_foreach(nkcmd, nuklear->Context(), nuklear->Commands())
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
    nk_clear(nuklear->Context());
}
