#include "stdafx.h"
#include "TextureHUDRenderer.h"
#include "DrawMesh.h"
#include "Shader/ShaderFactory.h"
#include "Mesh/Primitive/Square.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "GraphicsInterface/GICommandUtils.h"
#include "WindowsApp.h"


void TextureHUDRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size2D size, const Texture2D& texture)
{
    Size2D viewportSize = Size2D{ size.w / view->GetRenderSize().w, size.h / view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / view->GetRenderSize().w - 0.5f, pos.y / view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    DrawMesh element(&mesh);
    DrawElement face(ShaderFactory::MinTextureColor(), ShaderFactory::TextureVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;

    auto textureProxy = MakeRef(cmd->CreateTextureProxy(TextureParam(), texture));
    face.RegisterShaderResource(textureProxy, 10);

    element.AddDrawElement(face);
    element.Draw(cmd);
}

void TextureHUDRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size2D size, const GITextureProxy& texture, int index)
{
    Size2D viewportSize = Size2D{ size.w / view->GetRenderSize().w, size.h / view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / view->GetRenderSize().w - 0.5f, pos.y / view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(viewportSize);
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    DrawMesh element(&mesh);

    ObjectBuffer* buffer = new ObjectBuffer;
    buffer->World = XMMatrixTranspose(mesh.GetMatrix());
    element.RegisterConstantBuffer(buffer, 0, ShaderType::VS);

    DrawElement face(ShaderFactory::MinTextureColor(), ShaderFactory::TextureVertexShader());
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
    element.Draw(cmd);
}
