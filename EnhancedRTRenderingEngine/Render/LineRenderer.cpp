#include "stdafx.h"
#include "LineRenderer.h"
#include "TextureEffects.h"

#include "DrawMesh.h"
#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

void LineRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, const std::vector<Line>& lines) {

    cmd->SetViewport(view->GetViewPortCfg());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), sizeof(hConstantBuffer), &hConstantBuffer));

    cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());

    auto lineMesh = std::make_shared<MeshBase<Vertex3D>>();
    
    for (auto && line : lines) {
        Vector4D color = Vector4D(line.color.r, line.color.g, line.color.b);
        Vertex3D bvert{ line.bpos, color, Vector2D(0, 0) };
        Vertex3D evert{ line.epos, color, Vector2D(0, 0) };

        lineMesh->AddVertex(bvert);
        lineMesh->AddVertex(evert);
    }

    lineMesh->SetPrimitiveType(VertexPrimitiveType::LINELIST);

    MeshObject<Vertex3D> mesh(lineMesh);
    DrawMesh element(&mesh);
    DrawElement face(ShaderFactory::MinPixelColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;
    element.AddDrawElement(face);
    element.Draw(cmd);
}