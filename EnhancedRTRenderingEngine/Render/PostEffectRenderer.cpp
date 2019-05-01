
#include "PostEffectRenderer.h"
#include "DrawMesh.h"
#include "Shader/ShaderFactory.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowsApp.h"
#include "GraphicsInterface/GICommandUtils.h"
#include <chrono>

void PostEffectRenderer::Apply(GIImmediateCommands* cmd, GIRenderView* view, const std::string& effect) {
    struct ConstantBuffer
    {
        float time;
        Size2D size;
    };

    static float timestamp = 0;
    AlignedBuffer<ConstantBuffer> buf;
    buf.time = timestamp++;
    //buf.time = static_cast<float>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    buf.size = view->GetRenderSize();

    Vector4D ClearColor{ 0.7f, 0.7f, 0.7f, 1.0f };

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);
    cmd->ClearRenderTargetView(view->GetOMResource()->GetMainRTV().get(), ClearColor);

    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(buf);
    auto cb = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc, &buf));
    cmd->PSSetConstantBuffers(0, cb.get());

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = MakeRef(SceneUtils::CreatePrimitiveMeshObject<PrimitiveMesh::Square<TexVertex>>(Size2D(1.0f, 1.0f)));
    mesh->SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    DrawMesh element(cmd, mesh->GetMesh());
    Material material;

    material.shadingType = ShadingType::Unlit;
    material.pShader = ResourceLoader::LoadShader(effect);
    material.vShader = ShaderFactory::TextureVertexShader();

    DrawElement face(&element, static_cast<unsigned int>(mesh->GetMesh()->GetVertexCount()), 0);
    face.SetShaders(Shader(ShadingType::Unlit, ResourceLoader::LoadShader(effect)), ShaderFactory::TextureVertexShader());
    face.Draw(cmd);
}