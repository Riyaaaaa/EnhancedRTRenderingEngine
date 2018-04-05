#include "stdafx.h"

#include "UnlitRenderer.h"
#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Common/Common.h"

using namespace DirectX;

template<class VertType>
void UnlitRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<MeshObject<VertType>>& meshes) {
    
    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get());

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), sizeof(hConstantBuffer), &hConstantBuffer));
    cmd->VSSetConstantBuffers(0, buffer.get());

    for (auto && object : meshes) {
        DrawMesh element(&object);
        DrawElement face(ShaderFactory::MinPixelColor(), ShaderFactory::MinVertexColor());

        face.startIndex = 0;
        if (object.GetMesh()->HasIndexList()) {
            face.faceNumVerts = object.GetMesh()->GetIndexList().size();
        }
        else {
            face.faceNumVerts = object.GetMesh()->GetVertexList().size();
        }
        element.AddDrawElement(face);
        element.Draw(cmd);
    }
}

template void UnlitRenderer::render<Vertex3D>(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<MeshObject<Vertex3D>>& meshes);
