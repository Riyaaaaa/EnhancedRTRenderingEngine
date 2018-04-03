#include "stdafx.h"
#include "PostEffectRenderer.h"
#include "DrawMesh.h"
#include "Shader/ShaderFactory.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"
#include <chrono>

void PostEffectRenderer::Apply(GIImmediateCommands* cmd, GIRenderView* view, const std::string& effect) {
    struct ConstantBuffer
    {
        float time;
        Size size;
    };

    static float timestamp = 0;
    AlignedBuffer<ConstantBuffer> buf;
    buf.time = timestamp++;
    //buf.time = static_cast<float>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    buf.size = view->GetRenderSize();

    Vector4D ClearColor{ 0.7f, 0.7f, 0.7f, 1.0f };
    cmd->SetViewport(view->GetViewPortCfg());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);
    cmd->ClearRenderTargetView(view->GetOMResource()->GetMainRTV().get(), ClearColor);

    auto cb = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, sizeof(float), sizeof(buf), &buf));
    cmd->PSSetConstantBuffers(0, cb.get());

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(Size(1.0f, 1.0f));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    DrawMesh element(&mesh);
    DrawElement face(Shader(ShadingType::Unlit, ResourceLoader::LoadShader(effect)), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;
    element.AddDrawElement(face);
    element.Draw(cmd);
}