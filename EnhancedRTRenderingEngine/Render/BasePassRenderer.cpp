#include "stdafx.h"
#include "BasePassRenderer.h"

#include "DrawMesh.h"

#include "Constant/RenderTag.h"
#include "Utility/SceneUtils.h"

#include "GraphicsInterface/GICommandUtils.h"

#include "WindowsApp.h"
#include "Common/Common.h"

using namespace DirectX;

D3D11BasePassRenderer::D3D11BasePassRenderer(GIImmediateCommands* cmd) {
    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(ConstantBuffer);
    constantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
    desc.byteWidth = sizeof(MaterialBuffer);
    materialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc));
    desc.byteWidth = sizeof(ObjectBuffer);
    objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
}

void D3D11BasePassRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* _scene) {
    Scene* scene = _scene->GetSourceScene();

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, view->GetOMResource()->depthStencilView);
    cmd->ClearDepthStencilView(view->GetOMResource()->depthStencilView.get(), 1.0f, 0);
    
    ConstantBuffer hConstantBuffer = SceneUtils::CreateBasePassConstantBuffer(scene);

    cmd->UpdateSubresource(constantBuffer.get(), &hConstantBuffer, sizeof(hConstantBuffer));

    cmd->VSSetConstantBuffers(0, constantBuffer.get());
    cmd->PSSetConstantBuffers(0, constantBuffer.get());

    // todo: support multi lights
    if (hConstantBuffer.numDirecitonalLights > 0) {
        cmd->PSSetShaderResources(0, _scene->GetDirectionalShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(0, _scene->GetDirectionalShadow(0)->GetSampler().get());
    }
    
    if (hConstantBuffer.numPointLights > 0) {
        cmd->PSSetShaderResources(1, _scene->GetPointShadow(0)->GetSubResourceView().get());
        cmd->PSSetSamplers(1, _scene->GetPointShadow(0)->GetSampler().get());
    }


    ObjectBuffer buffer;
    for (auto && object : scene->GetViewObjects()) {
        auto& mesh = object.GetMesh();
        DrawMesh element(cmd, &object);

        buffer.World = XMMatrixTranspose(object.GetMatrix());
        buffer.NormalWorld = XMMatrixInverse(nullptr, object.GetMatrix());

        cmd->UpdateSubresource(objectBuffer.get(), &buffer, sizeof(buffer));

        element.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

        if (object.HasReflectionSource()) {
            auto& tex = _scene->GetEnviromentMap(object.GetReflectionSourceId());
            cmd->PSSetShaderResources(2, tex->GetSubResourceView().get());
            cmd->PSSetSamplers(2, tex->GetSampler().get());
        }

        int index = 0;
        for (auto && drawface : mesh->GetDrawFacesMap()) {
            auto& material = object.GetMaterials()[drawface.materialIdx];
            
            TextureParam param;
            param.type = material.type;
            GITextureProxy texture = GITextureProxyEntity::Create();
            if (material.type == TextureType::Texture2D) {
                param.arraySize = 1;
                texture->Initialize(cmd, param, material.texture);
            }
            else if (material.type == TextureType::TextureCube) {
                param.arraySize = 6;
                texture->Initialize(cmd, param, material.cubeTexture.textures);
            }

            MaterialBuffer mbuf{ material.metallic, material.roughness };
            cmd->UpdateSubresource(materialBuffer.get(), &mbuf, sizeof(mbuf));

            Shader ps(material.shadingType, material.pShader);
            ps.constantBuffers.emplace_back(materialBuffer, 1);
            ps.textureResources.emplace_back(texture, 10);
            
            DrawElement face(ps, Shader(ShadingType::Vertex, material.vShader));
            face.faceNumVerts = drawface.faceNumVerts;
            face.startIndex = index;

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
