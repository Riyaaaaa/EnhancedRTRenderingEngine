#include "stdafx.h"
#include "TextureEffectRenderer.h"
#include "Mesh/Primitive/Square.h"

#include "GraphicsInterface/GIImmediateCommands.h"
#include "DrawMesh.h"

#include "Shader/ShaderFactory.h"
#include "Scene/MeshObject.h"
#include "Utility/SceneUtils.h"
#include "WindowManager.h"

std::shared_ptr<GITexture2D> TextureEffectRenderer::Apply(GIImmediateCommands* cmd, const GITextureProxy& src, const std::string& effect) {
    auto param = src->GetTexture()->GetTextureParam();
    
    param.bindFlag |= TextureBindTarget::RENDER_TARGET;

    auto dstTex = MakeRef(cmd->CreateTexture2D(param));

    SamplerParam sparam;
    sparam.addressMode = TextureAddressMode::CLAMP;

    ViewportCfg vcfg;
    vcfg.height = param.height;
    vcfg.width = param.width;
    vcfg.topLeftX = 0;
    vcfg.topLeftY = 0;
    vcfg.maxDepth = 1.0f;
    vcfg.minDepth = 0.0f;

    Vector4D ClearColor{ 0.7f, 0.7f, 0.7f, 1.0f };
    cmd->SetViewport(vcfg);
    auto rtv = MakeRef(cmd->CreateRenderTargetView(dstTex.get()));

    cmd->OMSetRenderTargets(std::vector<std::shared_ptr<GIRenderTargetView>>{rtv}, nullptr);
    cmd->ClearRenderTargetView(rtv.get(), ClearColor);

    Vector2D viewportPos = Vector2D{ 0.0f, 0.0f };
    auto mesh = SceneUtils::CreatePrimitiveMeshObject<Square<TexVertex>>(Size(1.0f, 1.0f));
    mesh.SetLocation(Vector3D{ viewportPos.x, viewportPos.y, 0.0f });

    DrawMesh element(&mesh);
    DrawElement face(Shader(ShadingType::Unlit, ResourceLoader::LoadShader(effect)), ShaderFactory::HUDVertexShader());
    face.faceNumVerts = mesh.GetMesh()->GetVertexCount();
    face.startIndex = 0;
    face.RegisterShaderResource(src, 0);
    element.AddDrawElement(face);
    element.Draw(cmd);

    return dstTex;
}
