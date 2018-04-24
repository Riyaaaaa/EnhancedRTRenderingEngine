#include "stdafx.h"
#include "RenderScene.h"

#include "Constant/RenderConfig.h"

#include "GraphicsInterface/GIImmediateCommands.h"

RenderScene::~RenderScene()
{
}

void RenderScene::Preprocess(GIImmediateCommands* cmd) {
    cmd->PSSetShaderResources(3, _lightMap->GetSubResourceView().get());
    cmd->PSSetSamplers(3, _lightMap->GetSampler().get());
}

void RenderScene::Refresh(GIImmediateCommands* cmd) {
    if (_scene->Dirty()) {
        if (_scene->LightDirty()) {
            _directionalShadows.clear();
            _pointShadows.clear();

            _directionalShadows.resize(_scene->GetDirectionalLights().size());
            _pointShadows.resize(_scene->GetPointLights().size());

            for (auto && pLight : _scene->GetPointLights()) {
                pLight.SetDirty(true);
            }
            _scene->SetLightDirty(false);
        }
        
        if (_scene->MeshDirty()) {
            for (auto && reflectionCapture : _scene->GetReflectionCaptures()) {
                if (_enviromentMaps.find(reflectionCapture->GetID()) == _enviromentMaps.end()) {
                    _enviromentMaps.insert(std::make_pair(reflectionCapture->GetID(), std::make_shared<GITextureProxyEntity>()));
                }

                reflectionCapture->SetupTexture(cmd, _enviromentMaps.at(reflectionCapture->GetID()));
            }

            _staticDrawMeshes.clear();
            _drawList.clear();
            for (auto && viewObject : _scene->GetViewObjects()) {
                auto& mesh = viewObject->GetMesh();
                viewObject->FindPrecisionReflectionSource(_scene->GetReflectionCaptures());

                DrawMesh draw_mesh(cmd, viewObject);

                ObjectBuffer buffer;
                buffer.World = XMMatrixTranspose(viewObject->GetMatrix());
                buffer.NormalWorld = XMMatrixInverse(nullptr, viewObject->GetMatrix());

                BufferDesc desc;
                desc.byteWidth = sizeof(buffer);
                desc.stride = sizeof(float);

                auto objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &buffer));
                draw_mesh.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

                if (!viewObject->HasLightMap() && viewObject->HasReflectionSource()) {
                    auto& tex = GetEnviromentMap(viewObject->GetReflectionSourceId());
                    draw_mesh.RegisterTexture(tex, 2);
                }

                _staticDrawMeshes[viewObject->GetID()] = draw_mesh;

                if (viewObject->HasLightMap()) {
                    DrawMesh light_mesh(cmd, &viewObject->GetLightBuildData());
                    light_mesh.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

                    if (viewObject->HasReflectionSource()) {
                        auto& tex = GetEnviromentMap(viewObject->GetReflectionSourceId());
                        light_mesh.RegisterTexture(tex, 2);
                    }
                    _bakedLightMeshes[viewObject->GetID()] = light_mesh;
                }

                int index = 0;
                for (auto && drawface : mesh->GetDrawElementMap()) {
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
                    if (viewObject->HasLightMap()) {
                        mbuf.useLightMap = 1.0f;
                    }
                    desc.byteWidth = sizeof(mbuf);
                    desc.stride = sizeof(float);
                    auto materialBuffer = MakeRef(cmd->CreateBuffer(ResourceType::PSConstantBuffer, desc, &mbuf));
                    cmd->UpdateSubresource(materialBuffer.get(), &mbuf, sizeof(mbuf));

                    Shader ps(material.shadingType, material.pShader);
                    ps.constantBuffers.emplace_back(materialBuffer, 1);
                    ps.textureResources.emplace_back(texture, 10);

                    DrawMesh* parent;
                    if (viewObject->HasLightMap()) {
                        parent = &_bakedLightMeshes[viewObject->GetID()];
                    }
                    else {
                        parent = &_staticDrawMeshes[viewObject->GetID()];
                    }
                    
                    DrawElement face(parent, drawface.faceNumVerts, index);
                    face.SetShaders(ps, Shader(ShadingType::Vertex, material.vShader));
                    _drawList.push_back(face);

                    index += drawface.faceNumVerts;
                }
            }

            _scene->SetMeshDirty(false);
        }
    }
}
