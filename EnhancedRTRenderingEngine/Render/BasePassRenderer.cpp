#include "stdafx.h"
#include "BasePassRenderer.h"

#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "Utility/SceneUtils.h"

#include "WindowManager.h"
#include "Common.h"

using namespace DirectX;

void D3D11BasePassRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* _scene) {
    Scene* scene = _scene->GetSourceScene();
    cmd->SetViewport(view->GetViewPortCfg());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);

    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);
    
    ConstantBuffer hConstantBuffer = SceneUtils::CreateBasePassConstantBuffer(scene);

    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), sizeof(ConstantBuffer), &hConstantBuffer));
    auto hpMaterialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, sizeof(float), sizeof(MaterialBuffer)));

    cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());
    cmd->PSSetConstantBuffers(0, hpConstantBuffer.get());

    // todo: support multi lights
    if (hConstantBuffer.numDirecitonalLights > 0) {
        cmd->PSSetShaderResources(0, _scene->GetDirectionalShadow(0).get());
        cmd->PSSetSamplers(0, _scene->GetDirectionalShadow(0)->GetSampler().get());
    }
    
    if (hConstantBuffer.numPointLights > 0) {
        cmd->PSSetShaderResources(1, _scene->GetPointShadow(0).get());
        cmd->PSSetSamplers(1, _scene->GetPointShadow(0)->GetSampler().get());
    }

    for (auto && object : scene->GetViewObjects()) {
        auto& mesh = object.GetMesh();
        DrawMesh element(&object);

        if (object.HasReflectionSource()) {
            auto& tex = _scene->GetEnviromentMap(object.GetReflectionSourceId());
            cmd->PSSetShaderResources(2, tex.get());
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
    
    /*ID3D11ShaderResourceView*   pNullSRV = nullptr;
    ID3D11SamplerState*         pNullSmp = nullptr;
    cmd->PSSetSamplers(0, 1, &pNullSmp);
    cmd->PSSetShaderResources(0, 1, &pNullSRV);
    cmd->PSSetSamplers(1, 1, &pNullSmp);
    cmd->PSSetShaderResources(1, 1, &pNullSRV);*/
    cmd->ResetPS();
}
