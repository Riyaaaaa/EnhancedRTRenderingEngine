#include "stdafx.h"
#include "D3D11TextureHUDRenderer.h"
#include "D3D11DrawElement.h"
#include "GraphicsInterface/GIShader.h"
#include "Shader/ShaderFactory.h"
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
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    _view->SetViewPortSize(_view->GetRenderSize());
    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    GIDrawElement element(&mesh);
    GIDrawFace face(ShaderFactory::MinTextureColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;

    D3D11TextureProxy textureProxy = std::make_shared<D3D11TextureProxyEntity>(_view->hpDevice);
    textureProxy->Initialize(TextureParam(), texture);

    face.RegisterShaderResource(textureProxy, 10);

    element.AddDrawFace(face);

    D3D11DrawElement drawer;
    drawer.Draw(_view, element);
}

void D3D11TextureHUDRenderer::render(Vector2D pos, Size size, const D3D11TextureProxy& texture, int index)
{
    Size viewportSize = Size{ size.w / _view->GetRenderSize().w, size.h / _view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    GIDrawElement element(&mesh);
    GIDrawFace face(ShaderFactory::MinTextureColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;

    TextureType type;
    D3D11_TEXTURE2D_DESC desc;
    texture->GetTexture().Get()->GetDesc(&desc);

    type = desc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE ? TextureType::TextureCube : TextureType::Texture2D;

    if (type == TextureType::TextureCube) {
        D3D11_TEXTURE2D_DESC faceDesc(desc);
        D3D11TextureProxy faceTexture = D3D11TextureProxyEntity::Create(_view->hpDevice);
        ComPtr<ID3D11Texture2D> faceTextureSrc;

        faceDesc.ArraySize = 1;
        faceDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        faceDesc.MiscFlags = 0;

        _view->hpDevice->CreateTexture2D(&faceDesc, nullptr, faceTextureSrc.ToCreator());
        
        _view->hpDeviceContext->CopySubresourceRegion(faceTextureSrc.Get(), 
            0, 0, 0, 0, texture->GetTexture().Get(), 
            D3D11CalcSubresource(0, index, desc.MipLevels), nullptr);

        faceTexture->Initialize(faceTextureSrc);
        face.RegisterShaderResource(faceTexture, 0);
    }
    else {
        face.RegisterShaderResource(texture, 0);
    }

    element.AddDrawFace(face);

    D3D11DrawElement drawer;
    drawer.Draw(_view, element);
}