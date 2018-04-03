#include "stdafx.h"
#include "TextureHUDRenderer.h"
#include "DrawElement.h"
#include "GraphicsInterface/GIDrawMesh.h"
#include "Shader/ShaderFactory.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"


void D3D11TextureHUDRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size size, const Texture2D& texture)
{
    Size viewportSize = Size{ size.w / view->GetRenderSize().w, size.h / view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / view->GetRenderSize().w - 0.5f, pos.y / view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    view->SetViewPortSize(cmd, view->GetRenderSize());

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    GIDrawMesh element(&mesh);
    GIDrawElement face(ShaderFactory::MinTextureColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;

    auto textureProxy = MakeRef(cmd->CreateTextureProxy(TextureParam(), texture));
    face.RegisterShaderResource(textureProxy, 10);

    element.AddDrawElement(face);

    D3D11DrawElement drawer;
    drawer.Draw(cmd, element);
}

void D3D11TextureHUDRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size size, const GITextureProxy& texture, int index)
{
    Size viewportSize = Size{ size.w / view->GetRenderSize().w, size.h / view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / view->GetRenderSize().w - 0.5f, pos.y / view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    GIDrawMesh element(&mesh);
    GIDrawElement face(ShaderFactory::MinTextureColor(), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;

    auto param = texture->GetTexture()->GetTextureParam();
    TextureType type = param.type;

    if (type == TextureType::TextureCube) {
        param.arraySize = 1;
        param.bindFlag = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        param.type = TextureType::Texture2D;

        auto faceTextureSrc = MakeRef(cmd->CreateTexture2D(param));
        cmd->CopyTexture2DFromArray(faceTextureSrc.get(), texture->GetTexture().get(), index, param.mipLevels);

        auto faceTexture = MakeRef(cmd->CreateTextureProxy(faceTextureSrc, SamplerParam()));
        face.RegisterShaderResource(faceTexture, 0);
    }
    else {
        face.RegisterShaderResource(texture, 0);
    }

    element.AddDrawElement(face);

    D3D11DrawElement drawer;
    drawer.Draw(cmd, element);
}
