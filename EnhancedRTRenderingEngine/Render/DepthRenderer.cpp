#include "stdafx.h"
#include "DepthRenderer.h"
#include "TextureEffects.h"

#include "DrawMesh.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

void D3D11DepthRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene)
{
    //todo: support multi lights;
    RenderDirectionalLightShadowMap(cmd, view, scene);
    RenderPointLightShadowMap(cmd, view, scene);
}

void D3D11DepthRenderer::RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* _scene) {
    auto* scene = _scene->GetSourceScene();

    auto& dLights = scene->GetDirectionalLights();

    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), sizeof(TransformBufferParam)));

    for (std::size_t i = 0; i < dLights.size(); i++) {
        auto& dLight = dLights[i];

        auto rtv = GICommandUtils::CreateRenderTargetView(cmd, dLight.GetShadowResolution(), TextureFormat::RGBA16_UNORM, true);
        auto dsv = GICommandUtils::CreateDepthStencilView(cmd, dLight.GetShadowResolution(), TextureFormat::D16_UNORM, true);

        view->SetViewPortSize(cmd, dLight.GetShadowResolution());

        cmd->OMSetRenderTargets(std::vector<std::shared_ptr<GIRenderTargetView>>{rtv}, dsv);
        Vector4D color{ 1.0f, 1.0f, 1.0f, 1.0f };
        cmd->ClearRenderTargetView(rtv.get(), color);
        cmd->ClearDepthStencilView(dsv.get(), 1.0f, 0);

        TransformBufferParam hConstantBuffer;
        hConstantBuffer.View = XMMatrixTranspose(dLight.GetViewProjection());
        hConstantBuffer.Projection = XMMatrixTranspose(dLight.GetPerspectiveProjection());

        cmd->UpdateSubresource(hpConstantBuffer.get(), &hConstantBuffer, 0);
        cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());

        for (auto && object : scene->GetViewObjects()) {
            DrawMesh element(&object);
            DrawElement face(ShaderFactory::RenderShadowMapShader(), ShaderFactory::DepthOnlyVertexShader());

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

        _scene->GetDirectionalShadow(i) = MakeRef(cmd->CreateTextureProxy(rtv->rtvTexture, SamplerParam()));
    }
}

void D3D11DepthRenderer::RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* _scene) {
    auto* scene = _scene->GetSourceScene();

    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), sizeof(TransformBufferParam)));

    auto& pLights = scene->GetPointLights();
    for (std::size_t i = 0; i < pLights.size(); i++) {
        auto& pLight = scene->GetPointLights()[i];

        if (!pLight.Dirty()) {
            return;
        }

        Size resolution = pLight.GetShadowResolution();
        GICommandUtils::SetViewportSize(cmd, resolution);

        std::vector<GIOMResource> target;

        for (int j = 0; j < 6; j++) {
            auto rtv = GICommandUtils::CreateRenderTargetView(cmd, resolution, TextureFormat::RGBA16_UNORM, true);
            auto dsv = GICommandUtils::CreateDepthStencilView(cmd, resolution, TextureFormat::D16_UNORM, true);

            target.push_back(GIOMResource(rtv, dsv));

            cmd->OMSetRenderTargets(target[j].renderTargets, target[j].depthStencilView);
            Vector4D color { 1.0f, 1.0f, 1.0f, 1.0f };
            cmd->ClearRenderTargetView(target[j].renderTargets[0].get(), color);
            cmd->ClearDepthStencilView(target[j].depthStencilView.get(), 1.0f, 0);

            TransformBufferParam hConstantBuffer;

            hConstantBuffer.View = XMMatrixTranspose(pLight.GetViewMatrix(static_cast<CUBE_DIRECTION>(j)));
            hConstantBuffer.Projection = XMMatrixTranspose(pLight.GetShadowPerspectiveMatrix());

            cmd->UpdateSubresource(hpConstantBuffer.get(), &hConstantBuffer, 0);
            cmd->VSSetConstantBuffers(0, hpConstantBuffer.get());

            for (auto && object : scene->GetViewObjects()) {
                DrawMesh element(&object);
                DrawElement face(ShaderFactory::RenderShadowMapShader(), ShaderFactory::DepthOnlyVertexShader());

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

        TextureParam param;
        param.format = TextureFormat::RGBA8_UNORM;
        param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
        param.type = TextureType::TextureCube;

        // Each element in the texture array has the same format/dimensions.
        auto srcParam = target[0].renderTargets[0]->rtvTexture->GetTextureParam();

        TextureParam texArrayDesc;
        texArrayDesc.width = srcParam.width;
        texArrayDesc.height = srcParam.height;
        texArrayDesc.mipLevels = srcParam.mipLevels;
        texArrayDesc.arraySize = 6;
        texArrayDesc.format = srcParam.format;
        texArrayDesc.type = TextureType::TextureCube;

        auto texCube = MakeRef(cmd->CreateTexture2D(texArrayDesc));
        for (UINT x = 0; x < 6; x++)
        {
            auto filtered = GaussianFilter(cmd, target[x].renderTargets[0]->rtvTexture);
            cmd->CopyTexture2D(texCube.get(), x, texArrayDesc.mipLevels, filtered.get());
        }

        _scene->GetPointShadow(i) = MakeRef(cmd->CreateTextureProxy(texCube, SamplerParam()));

        pLight.SetDirty(false);
    }
}
