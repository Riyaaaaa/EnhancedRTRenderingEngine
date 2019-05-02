
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

    auto mesh = SceneUtils::CreatePrimitiveMeshObject<PrimitiveMesh::Square<TexVertex>>(viewportSize);
    mesh->SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    DrawMesh element(cmd, mesh);

    Material material;
    material.shadingType = ShadingType::Unlit;
    material.pShader = ResourceLoader::LoadShader("MinTextureColor");
    material.vShader = ResourceLoader::LoadShader("TextureVertexShader");
    material.texture = texture;

    ElementDesc desc;
    desc.materialIdx = 0;
    desc.faceIdx = 0;
    desc.faceNumVerts = mesh->GetMesh()->GetVertexCount();
    element.ExtractDrawElements(cmd, { desc }, {material});
    element.Draw(cmd);
}

void TextureHUDRenderer::render(GIImmediateCommands* cmd, GIRenderView* view, Vector2D pos, Size2D size, const GITextureProxy& texture, int index)
{
    Size2D viewportSize = Size2D{ size.w / view->GetRenderSize().w, size.h / view->GetRenderSize().h };
    Vector2D viewportPos = Vector2D{ pos.x / view->GetRenderSize().w - 0.5f, pos.y / view->GetRenderSize().h - 0.5f } *2.0f;

    auto mesh = MakeRef(SceneUtils::CreatePrimitiveMeshObject<PrimitiveMesh::Square<TexVertex>>(viewportSize));
    mesh->SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    GICommandUtils::SetViewportSize(cmd, view->GetRenderSize());
    cmd->OMSetRenderTargets(view->GetOMResource()->renderTargets, nullptr);

    DrawMesh element(cmd, mesh.get());

    Material material;
    auto param = texture->GetTexture()->GetTextureParam();
    TextureType type = param.type;
    material.type = type;
    material.shadingType = ShadingType::Unlit;
    material.pShader = ResourceLoader::LoadShader("MinTextureColor");
    material.vShader = ResourceLoader::LoadShader("TextureVertexShader");

    ElementDesc desc;
    desc.materialIdx = 0;
    desc.faceIdx = 0;
    desc.faceNumVerts = mesh->GetMesh()->GetVertexCount();
    element.ExtractDrawElements(cmd, { desc }, { material });

    if (type == TextureType::TextureCube) {
        param.arraySize = 1;
        param.bindFlag = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
        param.type = TextureType::Texture2D;

        auto faceTextureSrc = MakeRef(cmd->CreateTexture2D(param));
        cmd->CopyTexture2DFromArray(faceTextureSrc.get(), texture->GetTexture().get(), index, param.mipLevels);

        auto faceTexture = MakeRef(cmd->CreateTextureProxy(faceTextureSrc, SamplerParam()));
        element.GetDrawElement(0)->PS().textureResources[UnlitMainTexture] = faceTexture;
    }
    else {
        element.GetDrawElement(0)->PS().textureResources[UnlitMainTexture] = texture;
    }

    element.Draw(cmd);
}
