#include "stdafx.h"
#include "D3D11BasePassRenderer.h"
#include "D3D11DrawElement.h"
#include "D3D11ConstantBufferBuilder.h"

#include "GraphicsInterface/GIDrawMesh.h"

#include "Constant/RenderTag.h"
#include "Utility/SceneUtils.h"

#include "WindowManager.h"
#include "Common.h"

using namespace DirectX;

D3D11BasePassRenderer::D3D11BasePassRenderer()
{
}


D3D11BasePassRenderer::~D3D11BasePassRenderer()
{
}

bool D3D11BasePassRenderer::Initialize(const std::shared_ptr<D3D11RenderView>& view) {
    _view = view;
    return true; 
}

void D3D11BasePassRenderer::render(D3D11SceneInfo* _scene) {
    if (!_view) {
        return;
    }

    Scene* scene = _scene->GetSourceScene();
    _view->SetViewPortSize(_view->GetRenderSize());
    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), _view->hpDepthStencilView.Get());

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);
    _view->hpDeviceContext->ClearDepthStencilView(_view->hpDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    
    ConstantBuffer hConstantBuffer = SceneUtils::CreateBasePassConstantBuffer(scene);
    ComPtr<ID3D11Buffer> hpConstantBuffer(nullptr), hpMaterialBuffer(nullptr);
    
    hpConstantBuffer = D3D11ConstantBufferBuilder::BuildConstantBuffer<ConstantBuffer>(_view->hpDevice, &hConstantBuffer);
    hpMaterialBuffer = D3D11ConstantBufferBuilder::BuildConstantBuffer<MaterialBuffer>(_view->hpDevice, nullptr);

    _view->hpDeviceContext->VSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());
    _view->hpDeviceContext->PSSetConstantBuffers(0, 1, hpConstantBuffer.Ref());

    // todo: support multi lights
    if (hConstantBuffer.numDirecitonalLights > 0) {
        _view->hpDeviceContext->PSSetShaderResources(0, 1, _scene->GetDirectionalShadow(0)->GetSubResourceView().Ref());
        _view->hpDeviceContext->PSSetSamplers(0, 1, _scene->GetDirectionalShadow(0)->GetSampler().Ref());
    }
    
    if (hConstantBuffer.numPointLights > 0) {
        _view->hpDeviceContext->PSSetShaderResources(1, 1, _scene->GetPointShadow(0)->GetSubResourceView().Ref());
        _view->hpDeviceContext->PSSetSamplers(1, 1, _scene->GetPointShadow(0)->GetSampler().Ref());
    }

    for (auto && object : scene->GetViewObjects()) {
        auto& mesh = object.GetMesh();
        GIDrawMesh element(&object);

        if (object.HasReflectionSource()) {
            auto& tex = _scene->GetEnviromentMap(object.GetReflectionSourceId());
            _view->hpDeviceContext->PSSetShaderResources(2, 1, tex->GetSubResourceView().Ref());
            _view->hpDeviceContext->PSSetSamplers(2, 1, tex->GetSampler().Ref());
        }

        int index = 0;
        for (auto && drawface : mesh->GetDrawFacesMap()) {
            auto& material = object.GetMaterials()[drawface.materialIdx];
            GIDrawElement face(material);
            face.faceNumVerts = drawface.faceNumVerts;
            face.startIndex = index;

            TextureParam param;
            param.type = material.type;
            D3D11TextureProxy texture = std::make_shared<D3D11TextureProxyEntity>(_view->hpDevice);
            if (material.type == TextureType::Texture2D) {
                texture->Initialize(param, material.texture);
            }
            else if (material.type == TextureType::TextureCube) {
                texture->Initialize(param, material.cubeTexture.textures);
            }

            face.RegisterShaderResource(texture, 10);

            MaterialBuffer buf{ material.metallic, material.roughness };
            face.RegisterConstantBuffer(&buf, 1);
            
            element.AddDrawElement(face);
            index += drawface.faceNumVerts;
        }

        D3D11DrawElement drawer;
        drawer.Draw(_view, element);
    }
    
    ID3D11ShaderResourceView*   pNullSRV = nullptr;
    ID3D11SamplerState*         pNullSmp = nullptr;
    _view->hpDeviceContext->PSSetSamplers(0, 1, &pNullSmp);
    _view->hpDeviceContext->PSSetShaderResources(0, 1, &pNullSRV);
    _view->hpDeviceContext->PSSetSamplers(1, 1, &pNullSmp);
    _view->hpDeviceContext->PSSetShaderResources(1, 1, &pNullSRV);
    _view->hpDeviceContext->PSSetShader(nullptr, nullptr, 0);
}
