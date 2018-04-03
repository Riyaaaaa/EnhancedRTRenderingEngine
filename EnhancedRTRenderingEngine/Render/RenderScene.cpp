#include "stdafx.h"
#include "RenderScene.h"

#include "Constant/RenderConfig.h"

D3D11SceneInfo::~D3D11SceneInfo()
{
}

void D3D11SceneInfo::Refresh(GIImmediateCommands* cmd, Scene* scene) {
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

            for (auto && viewObject : scene->GetViewObjects()) {
                viewObject.FindPrecisionReflectionSource(scene->GetReflectionCaptures());
            }

            scene->SetMeshDirty(false);
        }
    }
}
