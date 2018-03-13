#include "stdafx.h"
#include "D3D11TextureHUDRenderer.h"
#include "D3D11DrawPlate.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"

bool D3D11TextureHUDRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;

    return true;
}

void D3D11TextureHUDRenderer::render(Vector2D pos, Size size, const Texture2D& texture)
{
    Size viewportSize = Size{ size.w / _view->GetRenderSize().w, size.h / _view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } * 2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f});

    _view->SetViewPortSize(_view->GetRenderSize());
    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    D3D11DrawPlate<TexVertex> element;
    D3D11TextureProxy tex(_view->hpDevice);
    tex.Initialize(texture.GetParam(), texture);
    element.Initialize(_view->hpDevice, &mesh, TextureType::Texture2D, "MinTextureColor", 0);
    element.SetTexture(tex);
    element.Draw(_view);
}

void D3D11TextureHUDRenderer::render(Vector2D pos, Size size, const D3D11TextureProxy& texture, int index)
{
    Size viewportSize = Size{ size.w / _view->GetRenderSize().w, size.h / _view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    D3D11DrawPlate<TexVertex> element;

    TextureType type;
    D3D11_TEXTURE2D_DESC desc;
    texture.GetTexture().Get()->GetDesc(&desc);

    type = desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? TextureType::TextureCube : TextureType::Texture2D;

    element.Initialize(_view->hpDevice, &mesh, TextureType::Texture2D, "MinTextureColor", index);

    if (type == TextureType::TextureCube) {
        D3D11_TEXTURE2D_DESC faceDesc(desc);
        D3D11TextureProxy faceTexture(_view->hpDevice);
        ComPtr<ID3D11Texture2D> faceTextureSrc;

        faceDesc.ArraySize = 1;
        faceDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        faceDesc.MiscFlags = 0;

        _view->hpDevice->CreateTexture2D(&faceDesc, nullptr, faceTextureSrc.ToCreator());
        
        _view->hpDeviceContext->CopySubresourceRegion(faceTextureSrc.Get(), 
            0, 0, 0, 0, texture.GetTexture().Get(), 
            D3D11CalcSubresource(0, index, desc.MipLevels), nullptr);

        faceTexture.Initialize(faceTextureSrc);

        element.SetTexture(faceTexture);
    }
    else {
        element.SetTexture(texture);
    }

    
    
    element.Draw(_view);
}