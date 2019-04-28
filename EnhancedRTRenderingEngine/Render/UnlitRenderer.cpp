

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

    for (auto && object : meshes) {
        DrawMesh mesh(cmd, object.get());
        ObjectBuffer buffer;
        buffer.World = XMMatrixTranspose(object->GetMatrix());
        buffer.NormalWorld = XMMatrixInverse(nullptr, object->GetMatrix());
        cmd->UpdateSubresource(objectBuffer.get(), &buffer, sizeof(buffer));
        mesh.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

        unsigned int faceNumVerts = 0;
        if (object->GetMesh()->HasIndexList()) {
            faceNumVerts = static_cast<unsigned int>(object->GetMesh()->GetIndexList().size());
        }
        else {
            faceNumVerts = static_cast<unsigned int>(object->GetMesh()->GetVertexList().size());
        }

        DrawElement element(&mesh, faceNumVerts, 0);
        element.SetShaders(ShaderFactory::MinPixelColor(), ShaderFactory::MinVertexColor());
        element.Draw(cmd);
    }
}

template void UnlitRenderer::render<Vertex3D>(GIImmediateCommands* cmd, GIRenderView* view, const CameraObject& camera, std::vector<std::unique_ptr<MeshObject<Vertex3D>>>& meshes);
