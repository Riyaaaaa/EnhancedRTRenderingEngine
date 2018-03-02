#include "stdafx.h"
#include "D3D11TextureHUDRenderer.h"
#include "D3D11DrawElement.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"

bool D3D11TextureHUDRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;

    return true;
}

void D3D11TextureHUDRenderer::render(Vector2D pos, Size size, const ResourceHandle<Texture2D>& texture)
{
    Size viewportSize = Size{ size.w / _view->GetRenderSize().w, size.h / _view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } * 2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    Material material{ MaterialParameters{ "HUDVertexShader", "MinTextureColor", "", 0.0f, 0.0f }, texture };
    std::vector<Material> materials;
    materials.emplace_back(std::move(material));
    mesh.SetMaterial(std::move(materials));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f});

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    D3D11DrawElement<TexVertex> element;
    element.Initialize(_view->hpDevice, &mesh, HUDRenderTag);
    element.Draw(_view);
}

void D3D11TextureHUDRenderer::render(Vector2D pos, Size size, const D3D11Texture& texture)
{
    Size viewportSize = Size{ size.w / _view->GetRenderSize().w, size.h / _view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / _view->GetRenderSize().w - 0.5f, pos.y / _view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    Material material{ MaterialParameters{ "HUDVertexShader", "MinTextureColor", "", 0.0f, 0.0f }};
    std::vector<Material> materials;
    materials.emplace_back(std::move(material));
    mesh.SetMaterial(std::move(materials));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    _view->hpDeviceContext->OMSetRenderTargets(1, _view->hpRenderTargetView.Ref(), nullptr);

    D3D11DrawElement<TexVertex> element;
    element.Initialize(_view->hpDevice, &mesh, HUDRenderTag);
    element.SetTexture(texture, 0);
    element.Draw(_view);
}