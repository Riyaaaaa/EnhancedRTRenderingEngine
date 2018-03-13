#include "stdafx.h"
#include "D3D11TextureEffectRenderer.h"
#include "D3D11DrawPlate.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"

bool D3D11TextureEffectRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;

    return true;
}

D3D11Texture D3D11TextureEffectRenderer::Apply(const D3D11Texture& src, const std::string& effect) {
    D3D11_TEXTURE2D_DESC desc;
    src.GetTexture()->GetDesc(&desc);
    
    desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

    ComPtr<ID3D11Texture2D> dstTex;
    if (FAILED(_view->hpDevice->CreateTexture2D(&desc, nullptr, dstTex.ToCreator()))) {
        return D3D11Texture();
    }

    D3D11Texture dst(_view->hpDevice);
    dst.Initialize(dstTex);

    ComPtr<ID3D11RenderTargetView> rtv;

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->SetViewPortSize(Size(desc.Width, desc.Height));
    _view->hpDevice->CreateRenderTargetView(dstTex.Get(), nullptr, rtv.ToCreator());
    _view->hpDeviceContext->OMSetRenderTargets(1, rtv.Ref(), nullptr);
    _view->hpDeviceContext->ClearRenderTargetView(_view->hpRenderTargetView.Get(), ClearColor);

    if (_constantBuffer) {
        _view->hpDeviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.Ref());
    }

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(Size(1.0f, 1.0f));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    D3D11DrawPlate<TexVertex> element;
    element.Initialize(_view->hpDevice, &mesh, TextureType::Texture2D, effect, 0);
    element.SetTexture(src);
    element.Draw(_view);

    return dst;
}

