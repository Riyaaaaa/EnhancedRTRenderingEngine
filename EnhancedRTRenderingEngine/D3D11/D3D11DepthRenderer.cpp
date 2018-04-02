#include "stdafx.h"
#include "D3D11DepthRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"
#include "D3D11TextureEffects.h"

#include "GraphicsInterface/GIDrawMesh.h"
#include "GraphicsInterface/GICommandUtils.h"

#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

void D3D11DepthRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* scene)
{
    //todo: support multi lights;
    RenderDirectionalLightShadowMap(cmd, view, scene);
    RenderPointLightShadowMap(cmd, view, scene);
}

void D3D11DepthRenderer::RenderDirectionalLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* _scene) {
    auto* scene = _scene->GetSourceScene();

    auto& dLights = scene->GetDirectionalLights();

    auto hpConstantBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, sizeof(float), nullptr, sizeof(TransformBufferParam)));

    for (std::size_t i = 0; i < dLights.size(); i++) {
        auto& dLight = dLights[i];

        auto rtv = GICommandUtils::CreateRenderTargetView(cmd, dLight.GetShadowResolution(), TextureFormat::RGBA16_UNORM, true);
        auto dsv = GICommandUtils::CreateDepthStencilView(cmd, dLight.GetShadowResolution(), TextureFormat::R16_TYPELESS, true);

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
            GIDrawMesh element(&object);
            GIDrawElement face(ShaderFactory::RenderShadowMapShader(), ShaderFactory::DepthOnlyVertexShader());

            face.startIndex = 0;
            if (object.GetMesh()->HasIndexList()) {
                face.faceNumVerts = object.GetMesh()->GetIndexList().size();
            }
            else {
                face.faceNumVerts = object.GetMesh()->GetVertexList().size();
            }
            element.AddDrawElement(face);
            D3D11DrawElement draw;
            draw.Draw(cmd, element);
        }

        _scene->GetDirectionalShadow(i) =  D3D11GaussianFilter(cmd, rtv->rtvTexture.get());
    }
}

void D3D11DepthRenderer::RenderPointLightShadowMap(GIImmediateCommands* cmd, GIRenderView* view, D3D11SceneInfo* _scene) {
    auto* scene = _scene->GetSourceScene();

    D3D11_BUFFER_DESC bufferDesc;
    ComPtr<ID3D11Buffer> hpConstantBuffer(nullptr);
    bufferDesc.ByteWidth = sizeof(TransformBufferParam);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);
    if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, NULL, hpConstantBuffer.ToCreator()))) {
        return;
    }

    auto& pLights = scene->GetPointLights();
    for (std::size_t i = 0; i < pLights.size(); i++) {
        auto& pLight = scene->GetPointLights()[i];

        if (!pLight.Dirty()) {
            return;
        }

        Size resolution = pLight.GetShadowResolution();
        _view->SetViewPortSize(resolution);
        std::vector<D3D11OMResource> target;
        for (int j = 0; j < 6; j++) {
            target.push_back(D3D11OMResource(_view->hpDevice, resolution));
            target[j].InitializeRenderTarget(_view->hpDeviceContext, true);
            target[j].InitializeDepthStencilView(_view->hpDeviceContext, true);

            _view->hpDeviceContext->OMSetRenderTargets(1, target[j].GetRenderTargetRef(), target[j].GetDepthStencilView());
            float color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            _view->hpDeviceContext->ClearRenderTargetView(target[j].GetRenderTarget(), color);
            _view->hpDeviceContext->ClearDepthStencilView(target[j].GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

            TransformBufferParam hConstantBuffer;

            hConstantBuffer.View = XMMatrixTranspose(pLight.GetViewMatrix(static_cast<CUBE_DIRECTION>(j)));
            hConstantBuffer.Projection = XMMatrixTranspose(pLight.GetShadowPerspectiveMatrix());

            _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer.Get(), 0, NULL, &hConstantBuffer, 0, 0);
            _view->hpDeviceContext->VSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());

            for (auto && object : scene->GetViewObjects()) {
                GIDrawMesh element(&object);
                GIDrawElement face(ShaderFactory::RenderShadowMapShader(), ShaderFactory::DepthOnlyVertexShader());

                face.startIndex = 0;
                if (object.GetMesh()->HasIndexList()) {
                    face.faceNumVerts = object.GetMesh()->GetIndexList().size();
                }
                else {
                    face.faceNumVerts = object.GetMesh()->GetVertexList().size();
                }
                element.AddDrawElement(face);
                D3D11DrawElement draw;
                draw.Draw(_view, element);
            }
        }

        TextureParam param;
        param.format = TextureFormat::RGBA8_UNORM;
        param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
        param.type = TextureType::TextureCube;

        // Each element in the texture array has the same format/dimensions.
        D3D11_TEXTURE2D_DESC texElementDesc;
        target[0].GetRTVTexture()->GetTexture().Get()->GetDesc(&texElementDesc);

        D3D11_TEXTURE2D_DESC texArrayDesc;
        texArrayDesc.Width = texElementDesc.Width;
        texArrayDesc.Height = texElementDesc.Height;
        texArrayDesc.MipLevels = texElementDesc.MipLevels;
        texArrayDesc.ArraySize = 6;
        texArrayDesc.Format = texElementDesc.Format;
        texArrayDesc.SampleDesc.Count = 1;
        texArrayDesc.SampleDesc.Quality = 0;
        texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
        texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texArrayDesc.CPUAccessFlags = 0;
        texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

        ComPtr<ID3D11Texture2D> texArray(nullptr);
        if (FAILED(_view->hpDevice->CreateTexture2D(&texArrayDesc, 0, texArray.ToCreator())))
            return;

        for (UINT x = 0; x < 6; x++)
        {
            cmd->CopySubresourceRegion(texArray.Get(), D3D11CalcSubresource(0, x, texArrayDesc.MipLevels), 0, 0, 0, D3D11GaussianFilter(_view, target[x].GetRTVTexture())->GetTexture().Get(), 0, nullptr);
        }

        D3D11TextureProxy tex = D3D11TextureProxyEntity::Create(_view->hpDevice);
        tex->Initialize(cmd, texArray);
        _scene->GetPointShadow(i) = tex;

        pLight.SetDirty(false);
    }
}
