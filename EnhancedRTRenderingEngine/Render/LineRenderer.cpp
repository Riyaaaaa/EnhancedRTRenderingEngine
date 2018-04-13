#include "stdafx.h"
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
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);

    view->SetRasterizerState(cmd, RasterizerState::CullNone);

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(TransformBufferParam);
    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &hConstantBuffer));

    cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());

    auto lineMesh = std::make_shared<MeshBase<LineVertex>>();
    
    for (auto && line : lines) {
        Vector4D color = Vector4D(line.color.r, line.color.g, line.color.b);
        LineVertex bvert{ line.seg.bpos, color, line.thickness };
        LineVertex evert{ line.seg.epos, color, line.thickness };

        lineMesh->AddVertex(bvert);
        lineMesh->AddVertex(evert);
    }

    lineMesh->SetPrimitiveType(VertexPrimitiveType::LINELIST);

    MeshObject<LineVertex> mesh(lineMesh);
    DrawMesh element(cmd, &mesh);

    ObjectBuffer buffer;
    buffer.World = XMMatrixTranspose(mesh.GetMatrix());

    desc.byteWidth = sizeof(buffer);
    desc.stride = sizeof(float);
    auto hpBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &buffer));
    element.RegisterConstantBuffer(hpBuffer, 0, ShaderType::VS);

    DrawElement face(&element, static_cast<unsigned int>(mesh.GetMesh()->GetVertexCount()), 0);
    face.SetShaders(ShaderFactory::MinPixelShader(), ShaderFactory::LineVertexShader(), ShaderFactory::LineGeometryShader());
    face.Draw(cmd);

    cmd->PSSetShader(nullptr);
    cmd->GSSetShader(nullptr);
    view->SetRasterizerState(cmd, RasterizerState::Default);
}
