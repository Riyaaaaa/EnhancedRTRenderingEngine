
#include "LineRenderer.h"
#include "TextureEffects.h"

#include "DrawMesh.h"
#include "Constant/RenderTag.h"

#include "GraphicsInterface/GICommandUtils.h"

#include "Common/Common.h"

using namespace DirectX;

void LineRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, const std::vector<Line>& lines) {
    if (lines.empty()) {
        return;
    }

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    //cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);

    view->SetRasterizerState(cmd, RasterizerState::CullNone);

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(TransformBufferParam);
    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &hConstantBuffer));

    cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());

    auto lineMesh = std::make_shared<Mesh<LineVertex>>();
    
    for (auto && line : lines) {
        Vector4D color = Vector4D(line.color.r, line.color.g, line.color.b);
        LineVertex bvert{ line.seg.bpos, color, line.thickness };
        LineVertex evert{ line.seg.epos, color, line.thickness };

        lineMesh->AddVertex(bvert);
        lineMesh->AddVertex(evert);
    }

    lineMesh->SetPrimitiveType(VertexPrimitiveType::LINELIST);

    auto mesh = std::make_shared< MeshObject<LineVertex>>((lineMesh));
    DrawMesh element(cmd, mesh->GetMesh());

    Material material;
    material.shadingType = ShadingType::Unlit;
    material.pShader = ShaderFactory::MinPixelShader();
    material.vShader = ShaderFactory::LineVertexShader();
    material.gShader = ShaderFactory::LineGeometryShader();

    ElementDesc edesc;
    edesc.faceIdx = 0;
    edesc.faceNumVerts = mesh->GetMesh()->GetVertexCount();

    element.ExtractDrawElements(cmd, { edesc }, { material });
    element.Draw(cmd);

    cmd->PSSetShader(nullptr);
    cmd->GSSetShader(nullptr);
    view->SetRasterizerState(cmd, RasterizerState::Default);
}
