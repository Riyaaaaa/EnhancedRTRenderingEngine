#include "stdafx.h"
#include "D3D11PostEffectRenderer.h"
#include "D3D11DrawPlate.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"
#include <chrono>


bool D3D11PostEffectRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;

    return true;
}

void D3D11PostEffectRenderer::Apply(const std::string& effect) {
    struct ConstantBuffer
    {
        float time;
        Size size;
    };

    static float timestamp = 0;
    AlignedBuffer<ConstantBuffer> buf;
    buf.time = timestamp++;
    //buf.time = static_cast<float>(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    buf.size = _view->GetRenderSize();

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->SetViewPortSize(_view->GetRenderSize());

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);

    auto cb = D3D11ConstantBufferBuilder::BuildConstantBuffer(_view->hpDevice, &buf);
    _view->hpDeviceContext->PSSetConstantBuffers(0, 1, cb.Ref());

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(Size(1.0f, 1.0f));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    D3D11DrawPlate<TexVertex> element;
    element.Initialize(_view->hpDevice, &mesh, TextureType::Texture2D, effect, 0);
    element.Draw(_view);
}