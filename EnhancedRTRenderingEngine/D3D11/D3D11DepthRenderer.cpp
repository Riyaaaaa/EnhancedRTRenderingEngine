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
    auto& target = _scene->GetDirectionalShadow(0);
    target.Initialize(_view->hpDevice, _view->hpDeviceContext, scene->GetDirectionalLights()[0].GetShadowTexture());

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

    hConstantBuffer.View = XMMatrixTranspose(scene->GetDirectionalLightViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());

    _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
    _view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

    for (auto && object : scene->GetViewObjects()) {
        D3D11DrawElement<Scene::VertType> element;
        element.Initialize(_view->hpDevice, &object, DepthRenderTag);
        element.Draw(_view);
    }

    SAFE_RELEASE(hpConstantBuffer);
}

void D3D11DepthRenderer::RenderPointLightShadowMap(D3D11Scene* _scene) {
    auto* scene = _scene->GetSourceScene();
    auto& target = _scene->GetPointShadow(0);
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
    
    for (int i = 0; i < 6; i++) {
        target.Initialize(_view->hpDevice, _view->hpDeviceContext, pLight.GetShadowTexture(static_cast<CUBE_DIRECTION>(i)));

        _view->hpDeviceContext->OMSetRenderTargets(0, nullptr, target.GetDepthStencilView().Get());
        _view->hpDeviceContext->ClearDepthStencilView(target.GetDepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        TransformBufferParam hConstantBuffer;

        hConstantBuffer.View = XMMatrixTranspose(pLight.GetViewMatrix(static_cast<CUBE_DIRECTION>(i)));
        hConstantBuffer.Projection = XMMatrixTranspose(pLight.GetShadowPerspectiveMatrix(static_cast<CUBE_DIRECTION>(i)));

        _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer, 0, NULL, &hConstantBuffer, 0, 0);
        _view->hpDeviceContext->VSSetConstantBuffers(0, 1, &hpConstantBuffer);

        for (auto && object : scene->GetViewObjects()) {
            D3D11DrawElement<Scene::VertType> element;
            element.Initialize(_view->hpDevice, &object, DepthRenderTag);
            element.Draw(_view);
        }
    }

    pLight.SetDirty(false);

    SAFE_RELEASE(hpConstantBuffer);
}
