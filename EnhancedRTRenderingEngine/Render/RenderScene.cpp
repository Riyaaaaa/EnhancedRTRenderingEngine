#include "stdafx.h"
#include "RenderScene.h"

#include "Constant/RenderConfig.h"


#include "GraphicsInterface/GIImmediateCommands.h"

RenderScene::~RenderScene()
{
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
            for (auto && viewObject : scene->GetViewObjects()) {
                viewObject.FindPrecisionReflectionSource(scene->GetReflectionCaptures());

                DrawMesh element(cmd, &viewObject);
                ObjectBuffer buffer;
                buffer.World = XMMatrixTranspose(viewObject.GetMatrix());
                buffer.NormalWorld = XMMatrixInverse(nullptr, viewObject.GetMatrix());

                BufferDesc desc;
                desc.byteWidth = sizeof(buffer);
                desc.stride = sizeof(float);

                auto objectBuffer = MakeRef(cmd->CreateBuffer(ResourceType::VSConstantBuffer, desc, &buffer));

                element.RegisterConstantBuffer(objectBuffer, 1, ShaderType::VS);

                _staticDrawMeshes[viewObject.GetID()] = element;
            }

            scene->SetMeshDirty(false);
        }
    }
}
