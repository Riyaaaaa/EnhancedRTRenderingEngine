#include "stdafx.h"
#include "D3D11TextureEffectRenderer.h"
#include "Mesh/Primitive/Square.h"
#include "D3D11DrawElement.h"
#include "GraphicsInterface/GIShader.h"
#include "Shader/ShaderFactory.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"

bool D3D11TextureEffectRenderer::Initialize(const std::shared_ptr<D3DX11RenderView>& view) {
    _view = view;

    return true;
}

D3D11TextureProxy D3D11TextureEffectRenderer::Apply(const D3D11TextureProxy& src, const std::string& effect) {
    D3D11_TEXTURE2D_DESC desc;
    src->GetTexture()->GetDesc(&desc);
    
    desc.BindFlags |= D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

    ComPtr<ID3D11Texture2D> dstTex;
    if (FAILED(_view->hpDevice->CreateTexture2D(&desc, nullptr, dstTex.ToCreator()))) {
        return D3D11TextureProxy();
    }

    SamplerParam param;
    param.addressMode = TextureAddressMode::CLAMP;
    D3D11TextureProxy dst = D3D11TextureProxyEntity::Create(_view->hpDevice);
    dst->Initialize(dstTex, param);

    ComPtr<ID3D11RenderTargetView> rtv;

    float ClearColor[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    _view->SetViewPortSize(Size(desc.Width, desc.Height));
    _view->hpDevice->CreateRenderTargetView(dstTex.Get(), nullptr, rtv.ToCreator());
    _view->hpDeviceContext->OMSetRenderTargets(1, rtv.Ref(), nullptr);
    _view->hpDeviceContext->ClearRenderTargetView(rtv.Get(), ClearColor);

    if (_constantBuffer) {
        _view->hpDeviceContext->PSSetConstantBuffers(0, 1, _constantBuffer.Ref());
    }

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(Size(1.0f, 1.0f));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    GIDrawElement element(&mesh);
    GIDrawFace face(Shader(ShadingType::Unlit, ResourceLoader::LoadShader(effect)), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;
    face.RegisterShaderResource(src, 0);
    element.AddDrawFace(face);
    D3D11DrawElement drawer;
    drawer.Draw(_view, element);

    return dst;
}

