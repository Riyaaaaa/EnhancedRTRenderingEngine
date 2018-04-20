#include "stdafx.h"
#include "RenderScene.h"

#include "Constant/RenderConfig.h"

#include "GraphicsInterface/GIImmediateCommands.h"

RenderScene::~RenderScene()
{
}

void RenderScene::Preprocess(GIImmediateCommands* cmd) {
}

void RenderScene::Refresh(GIImmediateCommands* cmd, Scene* scene) {
    _scene = scene;

    if (scene->Dirty()) {
        if (scene->LightDirty()) {
            _directionalShadows.clear();
            _pointShadows.clear();

            _directionalShadows.resize(scene->GetDirectionalLights().size());
            _pointShadows.resize(scene->GetPointLights().size());

            for (auto && pLight : scene->GetPointLights()) {
                pLight.SetDirty(true);
            }
            scene->SetLightDirty(false);
        }
        
        if (scene->MeshDirty()) {
            for (auto && reflectionCapture : scene->GetReflectionCaptures()) {
                if (_enviromentMaps.find(reflectionCapture->GetID()) == _enviromentMaps.end()) {
                    _enviromentMaps.insert(std::make_pair(reflectionCapture->GetID(), std::make_shared<GITextureProxyEntity>()));
                }

                reflectionCapture->SetupTexture(cmd, _enviromentMaps.at(reflectionCapture->GetID()));
            }

            _staticDrawMeshes.clear();
            _drawList.clear();
            for (auto && viewObject : scene->GetViewObjects()) {
                auto& mesh = viewObject->GetMesh();
                viewObject->FindPrecisionReflectionSource(scene->GetReflectionCaptures());

                DrawMesh draw_mesh(cmd, viewObject);
                ObjectBuffer buffer;
                buffer.World = XMMatrixTranspose(viewObject->GetMatrix());
                buffer.NormalWorld = XMMatrixInverse(nullptr, viewObject->GetMatrix());

                BufferDesc desc;
                desc.byteWidth = sizeof(buffer);
                desc.stride = sizeof(float);

                auto objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &buffer));
                draw_mesh.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

                if (viewObject->HasReflectionSource()) {
                    auto& tex = GetEnviromentMap(viewObject->GetReflectionSourceId());
                    draw_mesh.RegisterTexture(tex, 2);
                }

                _staticDrawMeshes[viewObject->GetID()] = draw_mesh;

                int index = 0;
                for (auto && drawface : mesh->GetDrawFacesMap()) {
                    auto& material = viewObject->GetMaterials()[drawface.materialIdx];

                    TextureParam param;
                    param.type = material.type;
                    GITextureProxy texture = GITextureProxyEntity::Create();
                    if (material.type == TextureType::Texture2D) {
                        param.arraySize = 1;
                        texture->Initialize(cmd, param, material.texture);
                    }
                    else if (material.type == TextureType::TextureCube) {
                        param.arraySize = 6;
                        texture->Initialize(cmd, param, material.cubeTexture.textures);
                    }

                    MaterialBuffer mbuf{ material.metallic, material.roughness };
                    desc.byteWidth = sizeof(mbuf);
                    desc.stride = sizeof(float);
                    auto materialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc, &mbuf));
                    cmd->UpdateSubresource(materialBuffer.get(), &mbuf, sizeof(mbuf));

                    Shader ps(material.shadingType, material.pShader);
                    ps.constantBuffers.emplace_back(materialBuffer, 1);
                    ps.textureResources.emplace_back(texture, 10);

                    DrawElement face(&_staticDrawMeshes[viewObject->GetID()], drawface.faceNumVerts, index);
                    face.SetShaders(ps, Shader(ShadingType::Vertex, material.vShader));

                    index += drawface.faceNumVerts;

                    _drawList.push_back(face);
                }
            }

            scene->SetMeshDirty(false);
        }
    }
}
