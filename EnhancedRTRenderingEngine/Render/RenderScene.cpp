
#include "RenderScene.h"

#include "Constant/RenderConfig.h"

#include "GraphicsInterface/GIImmediateCommands.h"

#include "UserData/UserConfig.h"

RenderScene::~RenderScene()
{
}

void  RenderScene::Notify(UserConfigEvent e) {
    switch (e) {
    case UserConfigEvent::ChangedLightMapSetting:
        _refreshTasks.push([this](GIImmediateCommands * cmd) {
            _scene->SetMeshDirty(true);
            });
        break;
    }
}

void RenderScene::Preprocess(GIImmediateCommands* cmd) {
    if (!_lightMap) {
        cmd->PSSetSamplers(3, MakeRef(cmd->CreateSamplerState(SamplerParam())).get());
        return;
    }

    cmd->PSSetShaderResources(3, _lightMap->GetSubResourceView().get());
    cmd->PSSetSamplers(3, _lightMap->GetSampler().get());
}

void RenderScene::Refresh(GIImmediateCommands* cmd) {
    while (!_refreshTasks.empty()) {
        _refreshTasks.front()(cmd);
        _refreshTasks.pop();
    }

    if (_scene->Dirty()) {
        if (_scene->LightDirty()) {
            _directionalShadows.clear();
            _pointShadows.clear();

            _directionalShadows.resize(_scene->GetDirectionalLights().size());
            _pointShadows.resize(_scene->GetPointLights().size());

            for (auto&& pLight : _scene->GetPointLights()) {
                pLight.SetDirty(true);
            }
            _scene->SetLightDirty(false);
        }

        if (_scene->MeshDirty()) {
            for (auto&& reflectionCapture : _scene->GetReflectionCaptures()) {
                if (!reflectionCapture->Dirty()) {
                    continue;
                }
                if (_enviromentMaps.find(reflectionCapture->GetID()) == _enviromentMaps.end()) {
                    _enviromentMaps.insert(std::make_pair(reflectionCapture->GetID(), std::make_shared<GITextureProxyEntity>()));
                }

                reflectionCapture->SetupTexture(cmd, _enviromentMaps.at(reflectionCapture->GetID()));
            }

            _staticDrawMeshes.clear();
            for (auto&& viewObject : _scene->GetViewObjects()) {
                auto& mesh = viewObject->GetMesh();

                viewObject->FindPrecisionReflectionSource(_scene->GetReflectionCaptures());

                DrawPlan plan;
                plan.useLightMap = viewObject->HasLightMap() && UserConfig::getInstance()->VisibleIndirectLights();
                plan.useEnviromentMap = viewObject->HasReflectionSource();

                DrawMesh* draw_mesh;
                draw_mesh->ExtractDrawElements(cmd, viewObject->GetMesh()->GetDrawElementMap(), viewObject->GetMaterials(), plan);

                if (viewObject->HasLightMap()) {
                    draw_mesh = new DrawMesh(cmd, viewObject->GetMesh());
                }
                else {
                    draw_mesh = new DrawMesh(cmd, &viewObject->GetLightBuildData());
                }

                if (viewObject->HasReflectionSource()) {
                    auto& tex = GetEnviromentMap(viewObject->GetReflectionSourceId());
                    draw_mesh->RegisterTexture(tex, 2);
                }

                _staticDrawMeshes[viewObject->GetID()] = std::unique_ptr<DrawMesh>(draw_mesh);
            }

            _scene->SetMeshDirty(false);
        }
    }
}

void RenderScene::Setup(GIImmediateCommands* cmd) {
    for (auto&& viewObject : _scene->GetViewObjects()) {
        ObjectBuffer buffer;
        buffer.World = XMMatrixTranspose(viewObject->GetMatrix());
        buffer.NormalWorld = XMMatrixInverse(nullptr, viewObject->GetMatrix());
    }
}

void RenderScene::RenderByBasePass(GIImmediateCommands* cmd)
{
    for (auto&& mesh : _staticDrawMeshes) {
        mesh.second->Draw(cmd);
    }

    for (auto&& mesh : _dynamicDrawMeshes) {
        mesh.second->Draw(cmd);
    }
}
