#include "stdafx.h"
#include "D3D11DepthRenderer.h"
#include "D3D11FormatUtils.h"
#include "D3D11DrawElement.h"

#include "Constant/RenderTag.h"

#include "Common.h"

using namespace DirectX;

bool D3D11DepthRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;
    return true;
}

void D3D11DepthRenderer::render(D3D11Scene* scene)
{
    //todo: support multi lights;
    RenderDirectionalLightShadowMap(scene);
    RenderPointLightShadowMap(scene);
}

void D3D11DepthRenderer::RenderDirectionalLightShadowMap(D3D11Scene* _scene) {
    auto* scene = _scene->GetSourceScene();
    auto& dLight = scene->GetDirectionalLights()[0];
    D3D11DepthStencilTarget target;

    _view->SetViewPortSize(dLight.GetShadowResolution());
    target.Initialize(_view->hpDevice, _view->hpDeviceContext, dLight.GetShadowResolution());

    _view->hpDeviceContext->OMSetRenderTargets(0, nullptr, target.GetDepthStencilView().Get());
    _view->hpDeviceContext->ClearDepthStencilView(target.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    D3D11_BUFFER_DESC bufferDesc;
    ID3D11Buffer* hpConstantBuffer = NULL;
    bufferDesc.ByteWidth = sizeof(TransformBufferParam);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);
    if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
        return;
    }

    TransformBufferParam hConstantBuffer;

    

    hConstantBuffer.View = XMMatrixTranspose(dLight.GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(dLight.GetPerspectiveProjection());

    _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
    _view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

    for (auto && object : scene->GetViewObjects()) {
        D3D11DrawElement<Scene::VertType> element;
        element.Initialize(_view->hpDevice, &object, DepthRenderTag);
        element.Draw(_view);
    }

    _scene->GetDirectionalShadow(0) = target.GetTexture();

    SAFE_RELEASE(hpConstantBuffer);
}

void D3D11DepthRenderer::RenderPointLightShadowMap(D3D11Scene* _scene) {
    auto* scene = _scene->GetSourceScene();
    auto& pLight = scene->GetPointLights()[0];

    if (!pLight.Dirty()) {
        return;
    }

    D3D11_BUFFER_DESC bufferDesc;
    ID3D11Buffer* hpConstantBuffer = NULL;
    bufferDesc.ByteWidth = sizeof(TransformBufferParam);
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);
    if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, NULL, &hpConstantBuffer))) {
        return;
    }
    
    Size resolution = pLight.GetShadowResolution();
    _view->SetViewPortSize(resolution);
    D3D11DepthStencilTarget target[6];
    for (int i = 0; i < 6; i++) {
        target[i].Initialize(_view->hpDevice, _view->hpDeviceContext, resolution);

        _view->hpDeviceContext->OMSetRenderTargets(0, nullptr, target[i].GetDepthStencilView().Get());
        _view->hpDeviceContext->ClearDepthStencilView(target[i].GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        TransformBufferParam hConstantBuffer;

        hConstantBuffer.View = XMMatrixTranspose(pLight.GetViewMatrix(static_cast<CUBE_DIRECTION>(i)));
        hConstantBuffer.Projection = XMMatrixTranspose(pLight.GetShadowPerspectiveMatrix());

        _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
        _view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

        for (auto && object : scene->GetViewObjects()) {
            D3D11DrawElement<Scene::VertType> element;
            element.Initialize(_view->hpDevice, &object, DepthRenderTag);
            element.Draw(_view);
        }
    }
    
    TextureParam param;
    param.format = TextureFormat::R16_TYPELESS;
    param.bindFlag = TextureBindTarget::SHADER_RESOURCE;
    param.type = TextureType::TextureCube;

    // Each element in the texture array has the same format/dimensions.
    D3D11_TEXTURE2D_DESC texElementDesc;
    target[0].GetTexture().GetTexture().Get()->GetDesc(&texElementDesc);

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

    ComPtr<ID3D11Texture2D> texArray = 0;
    if (FAILED(_view->hpDevice->CreateTexture2D(&texArrayDesc, 0, texArray.ToCreator())))
        return;

    // Copy individual texture elements into texture array.
    D3D11_BOX sourceRegion;

    //Here i copy the mip map levels of the textures
    for (UINT x = 0; x < 6; x++)
    {
        //for (UINT mipLevel = 0; mipLevel < texArrayDesc.MipLevels; mipLevel++)
        //{
        //    sourceRegion.left = 0;
        //    sourceRegion.right = (texArrayDesc.Width >> mipLevel);
        //    sourceRegion.top = 0;
        //    sourceRegion.bottom = (texArrayDesc.Height >> mipLevel);
        //    sourceRegion.front = 0;
        //    sourceRegion.back = 1;

        //    //test for overflow
        //    if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
        //        break;

        //    pd3dContext->CopySubresourceRegion(texArray.Get(), D3D11CalcSubresource(mipLevel, x, texArrayDesc.MipLevels), 0, 0, 0, target[x].GetTexture().GetTexture().Get(), mipLevel, &sourceRegion);
        //}
        _view->hpDeviceContext->CopySubresourceRegion(texArray.Get(), D3D11CalcSubresource(0, x, texArrayDesc.MipLevels), 0, 0, 0, target[x].GetTexture().GetTexture().Get(), 0, nullptr);
    }

    D3D11Texture tex;
    //tex.Initialize(_view->hpDevice, target[0].GetTexture().GetTexture());
    tex.Initialize(_view->hpDevice, texArray);
    _scene->GetPointShadow(0) = tex;

    pLight.SetDirty(false);

    SAFE_RELEASE(hpConstantBuffer);
}
