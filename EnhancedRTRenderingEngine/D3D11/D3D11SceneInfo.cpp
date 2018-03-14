#include "stdafx.h"
#include "D3D11SceneInfo.h"

#include "Constant/RenderConfig.h"

D3D11SceneInfo::~D3D11SceneInfo()
{
}

void D3D11SceneInfo::Refresh(ComPtr<ID3D11Device> device, Scene* scene) {
    _scene = scene;

    if (scene->Dirty()) {
        if (scene->LightDirty()) {
            _directionalShadows.clear();
            _pointShadows.clear();

            _directionalShadows.resize(scene->GetDirectionalLights().size(), D3D11TextureProxy(device));
            _pointShadows.resize(scene->GetPointLights().size(), D3D11TextureProxy(device));

            for (auto && pLight : scene->GetPointLights()) {
                pLight.SetDirty(true);
            }
            scene->SetLightDirty(false);
        }
        
        if (scene->MeshDirty()) {
            for (auto && reflectionCapture : scene->GetReflectionCaptures()) {
                if (_enviromentMaps.find(reflectionCapture->GetID()) == _enviromentMaps.end()) {
                    _enviromentMaps.insert(std::make_pair(reflectionCapture->GetID(), D3D11TextureProxy(device)));
                }

                reflectionCapture->SetupTexture(&_enviromentMaps.at(reflectionCapture->GetID()));
            }

            for (auto && viewObject : scene->GetViewObjects()) {
                viewObject.FindPrecisionReflectionSource(scene->GetReflectionCaptures());
            }

            scene->SetMeshDirty(false);
        }
    }
}
