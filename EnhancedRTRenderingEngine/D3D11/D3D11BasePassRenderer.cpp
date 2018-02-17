#include "stdafx.h"
#include "D3D11BasePassRenderer.h"
#include "D3D11DrawElement.h"

#include "../Resource/ResourceLoader.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"

#include "Constant/RenderTag.h"
#include "WindowManager.h"
#include "Common.h"

using namespace DirectX;

D3D11BasePassRenderer::D3D11BasePassRenderer()
{
}


D3D11BasePassRenderer::~D3D11BasePassRenderer()
{
}

bool D3D11BasePassRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;
    return true; 
}

void D3D11BasePassRenderer::render(Scene* scene) {
    if (!_view) {
        return;
    }

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), _view->hpDepthStencilView.Get());

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);
    _view->hpDeviceContext->ClearDepthStencilView(_view->hpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    D3D11_BUFFER_DESC bufferDesc;
    ComPtr<ID3D11Buffer> hpConstantBuffer = nullptr, hpMaterialBuffer = nullptr;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = sizeof(float);

    bufferDesc.ByteWidth = sizeof(ConstantBuffer);
    if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, nullptr, hpConstantBuffer.ToCreator()))) {
        return;
    }

    bufferDesc.ByteWidth = sizeof(MaterialBuffer);
    if (FAILED(_view->hpDevice->CreateBuffer(&bufferDesc, nullptr, hpMaterialBuffer.ToCreator()))) {
        return;
    }

    ConstantBuffer hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());
    hConstantBuffer.Shadow = XMMatrixTranspose(scene->GetDirectionalLightViewProjection());
    hConstantBuffer.Eye = scene->GetEyePoint();

    // Only support one light.
    hConstantBuffer.numDirecitonalLights = scene->GetDirectionalLights().size();
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numDirecitonalLights) {
            break;
        }
        hConstantBuffer.DirectionalLight[i] = scene->GetDirectionalLights()[0].GetDirection();
    }

    hConstantBuffer.numPointLights = scene->GetPointLightParams().size();
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numPointLights) {
            break;
        }
        hConstantBuffer.PointLight[i] = scene->GetPointLightParams()[0];
    }
    
    _view->hpDeviceContext->UpdateSubresource(hpConstantBuffer.Get(), 0, NULL, &hConstantBuffer, 0, 0);
    _view->hpDeviceContext->VSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());
    _view->hpDeviceContext->PSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());
    _view->hpDeviceContext->PSSetShaderResources(0, 1, _view->hpShadowMapTarget.GetSRV().Ref());
    _view->hpDeviceContext->PSSetSamplers(0, 1, _view->hpShadowMapTarget.GetSampler().Ref());

    for (auto && object : scene->GetViewObjects()) {
        D3D11DrawElement<Scene::VertType> element;
        element.Initialize(_view->hpDevice, &object, OpaqueRenderTag);
        element.Draw(_view);
    }
    
    ID3D11ShaderResourceView*   pNullSRV = nullptr;
    ID3D11SamplerState*         pNullSmp = nullptr;
    _view->hpDeviceContext->PSSetSamplers(0, 1, &pNullSmp);
    _view->hpDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
    _view->hpDeviceContext->PSSetSamplers(1, 1, &pNullSmp);
    _view->hpDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
    _view->hpDeviceContext->PSSetShader(nullptr, nullptr, 0);
    _view->hpDXGISwpChain->Present(0, 0);
}
