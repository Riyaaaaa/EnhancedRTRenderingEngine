
#include "DepthRenderer.h"
#include "TextureEffects.h"

#include "DrawMesh.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Constant/RenderTag.h"

#include "Common/Common.h"

using namespace DirectX;

D3D11DepthRenderer::D3D11DepthRenderer(GIImmediateCommands* cmd) {
    BufferDesc desc;
    desc.stride = sizeof(float);
    desc.byteWidth = sizeof(TransformBufferParam);
    transformBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
    desc.byteWidth = sizeof(ObjectBuffer);
    objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc));
}

void D3D11DepthRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* scene)
{
    //todo: support multi lights;
    RenderDirectionalLightShadowMap(cmd, view, scene);
    RenderPointLightShadowMap(cmd, view, scene);
}

void D3D11DepthRenderer::RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* renderScene) {
    auto* scene = renderScene->GetSourceScene();
    auto& dLights = scene->GetDirectionalLights();

    for (int i = 0; i < static_cast<int>(dLights.size()); i++) {
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

        cmd->UpdateSubresource(transformBuffer.get(), &hConstantBuffer, 0);
        cmd->VSSetConstantBuffers(0, transformBuffer.get());

        for (auto && object : scene->GetViewObjects()) {
            auto mesh = renderScene->GetStaticDrawMesh(object->GetID());

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

            DrawMesh drawMesh(cmd, object);
            Material material;
            material.shadingType = ShadingType::Detph;
            material.pShader = ResourceLoader::LoadShader("RenderShadowMapShader");
            material.vShader = ResourceLoader::LoadShader("DepthVertexShader");

            drawMesh.ExtractDrawElements(cmd, { desc }, {material});
            drawMesh.Draw(cmd);
        }

        renderScene->GetDirectionalShadow(i) = MakeRef(cmd->CreateTextureProxy(rtv->rtvTexture, SamplerParam()));
    }
}

void D3D11DepthRenderer::RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, RenderScene* renderScene) {
    auto* scene = renderScene->GetSourceScene();
    auto& pLights = scene->GetPointLights();

    for (int i = 0; i < static_cast<int>(pLights.size()); i++) {
        auto& pLight = scene->GetPointLights()[i];

        if (!pLight.Dirty()) {
            return;
        }

        Size2D resolution = pLight.GetShadowResolution();
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

            cmd->UpdateSubresource(transformBuffer.get(), &hConstantBuffer, 0);
            cmd->VSSetConstantBuffers(0, transformBuffer.get());

            for (auto&& object : scene->GetViewObjects()) {
                auto mesh = renderScene->GetStaticDrawMesh(object->GetID());

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

                DrawMesh drawMesh(cmd, object);
                Material material;
                material.shadingType = ShadingType::Detph;
                material.pShader = ResourceLoader::LoadShader("RenderShadowMapShader");
                material.vShader = ResourceLoader::LoadShader("DepthVertexShader");

                drawMesh.ExtractDrawElements(cmd, { desc }, { material });
                drawMesh.Draw(cmd);
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

        renderScene->GetPointShadow(i) = MakeRef(cmd->CreateTextureProxy(texCube, SamplerParam()));

        pLight.SetDirty(false);
    }
}
