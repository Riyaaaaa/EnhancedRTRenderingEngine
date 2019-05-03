

#include "UnlitRenderer.h"
#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Common/Common.h"

using namespace DirectX;

template<class VertType>
void UnlitRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<std::unique_ptr<MeshObject<VertType>>>& meshes) {
    
    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get());

    TransformBufferParam hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(camera.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(camera.GetPerspectiveProjection());

    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(hConstantBuffer);
    auto buffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &hConstantBuffer));
    cmd->VSSetConstantBuffers(0, buffer.get());

    desc.byteWidth = sizeof(ObjectBuffer);
    desc.stride = sizeof(float);
    auto objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));

    for (auto&& object : meshes) {
        ElementDesc desc;
        desc.materialIdx = 0;
        desc.faceIdx = 0;

        unsigned int faceNumVerts = 0;
        if (object->GetMesh()->HasIndexList()) {
            desc.faceNumVerts = static_cast<unsigned int>(object->GetMesh()->GetIndexList().size());
        }
        else {
            desc.faceNumVerts = static_cast<unsigned int>(object->GetMesh()->GetVertexList().size());
        }

        DrawMesh mesh(cmd, object.get());
        Material material;
        material.shadingType = ShadingType::Unlit;
        material.pShader = ResourceLoader::LoadShader("MinPixelShader");
        material.vShader = ResourceLoader::LoadShader("VertexShader");

        mesh.ExtractDrawElements(cmd, { desc }, { material });
        mesh.Draw(cmd);
    }
}

template void UnlitRenderer::render<Vertex3D>(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<std::unique_ptr<MeshObject<Vertex3D>>>& meshes);
