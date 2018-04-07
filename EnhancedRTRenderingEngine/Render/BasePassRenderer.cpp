#include "stdafx.h"
#include "BasePassRenderer.h"

#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "Utility/SceneUtils.h"

#include "GraphicsInterface/GICommandUtils.h"

#include "WindowManager.h"
#include "Common/Common.h"

using namespace DirectX;

void D3D11BasePassRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* _scene) {
    Scene* scene = _scene->GetSourceScene();

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);
    
    ConstantBuffer hConstantBuffer = SceneUtils::CreateBasePassConstantBuffer(scene);

    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(ConstantBuffer);
    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &hConstantBuffer));
    desc.byteWidth = sizeof(MaterialBuffer);
    auto hpMaterialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc));

    cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());
    cmd->PSSetConstantBuffers(0, hpConstantBuffer.get());

    // todo: support multi lights
    if (hConstantBuffer.numDirecitonalLights > 0) {
        cmd->PSSetShaderResources(0, _scene->GetDirectionalShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(0, _scene->GetDirectionalShadow(0)->GetSampler().get());
    }
    
    if (hConstantBuffer.numPointLights > 0) {
        cmd->PSSetShaderResources(1, _scene->GetPointShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(1, _scene->GetPointShadow(0)->GetSampler().get());
    }

    for (auto && object : scene->GetViewObjects()) {
        auto& mesh = object.GetMesh();
        DrawMesh element(&object);
        ObjectBuffer* buffer = new ObjectBuffer;
        buffer->World = XMMatrixTranspose(object.GetMatrix());
        buffer->NormalWorld = XMMatrixInverse(nullptr, object.GetMatrix());
        element.RegisterConstantBuffer(buffer, 1, ShaderType::VS);

        if (object.HasReflectionSource()) {
            auto& tex = _scene->GetEnviromentMap(object.GetReflectionSourceId());
            cmd->PSSetShaderResources(2, tex->GetSubResourceView().get());
            cmd->PSSetSamplers(2, tex->GetSampler().get());
        }

        int index = 0;
        for (auto && drawface : mesh->GetDrawFacesMap()) {
            auto& material = object.GetMaterials()[drawface.materialIdx];
            DrawElement face(material);
            face.faceNumVerts = drawface.faceNumVerts;
            face.startIndex = index;

            TextureParam param;
            param.type = material.type;
            GITextureProxy texture = GITextureProxyEntity::Create();
            if (material.type == TextureType::Texture2D) {
                texture->Initialize(cmd, param, material.texture);
            }
            else if (material.type == TextureType::TextureCube) {
                texture->Initialize(cmd, param, material.cubeTexture.textures);
            }

            face.RegisterShaderResource(texture, 10);

            MaterialBuffer buf{ material.metallic, material.roughness };
            face.RegisterConstantBuffer(&buf, 1, ShaderType::PS);
            
            element.AddDrawElement(face);
            index += drawface.faceNumVerts;
        }

        element.Draw(cmd);
    }
    
    cmd->PSSetSamplers(0, nullptr);
    cmd->PSSetShaderResources(0, nullptr);
    cmd->PSSetSamplers(1, nullptr);
    cmd->PSSetShaderResources(1, nullptr);
    cmd->ResetPS();
}
