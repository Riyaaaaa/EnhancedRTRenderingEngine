#pragma once

#include <unordered_map>
#include <queue>

#include "Algorithms/SpaceOctree.h"
#include "GraphicsInterface/GITextureProxy.h"
#include "Scene/Scene.h"

#include "System/Observer.h"

#include "UserData/SettingsEvent.h"

#include "DrawMesh.h"


class RenderScene : public ERTRESystem::Observer<UserConfigEvent>
{
public:
    RenderScene(Scene* scene) : _scene(scene) {}
    ~RenderScene();

    void Notify(UserConfigEvent e) override;

    void Preprocess(GIImmediateCommands* cmd);

    void Refresh(GIImmediateCommands* cmd);
    Scene* GetSourceScene() { return _scene; }
    GITextureProxy& GetDirectionalShadow(int index) {
        return _directionalShadows[index];
    }
    GITextureProxy& GetPointShadow(int index) {
        return _pointShadows[index];
    }
    GITextureProxy& GetEnviromentMap(std::size_t index) {
        return _enviromentMaps.at(index);
    }
    DrawMesh* GetStaticDrawMesh(std::size_t objectId) {
        return _staticDrawMeshes[objectId].get();
    }

    void RenderByBasePass(GIImmediateCommands* cmd);

protected:
    void Setup(GIImmediateCommands* cmd);
    Scene * _scene;
    std::vector<GITextureProxy> _directionalShadows;
    std::vector<GITextureProxy> _pointShadows;
    std::unordered_map<std::size_t, GITextureProxy> _enviromentMaps;

    // BasePass caches
    std::unordered_map<std::size_t, std::unique_ptr<DrawMesh>> _dynamicDrawMeshes;
    std::unordered_map<std::size_t, std::unique_ptr<DrawMesh>> _staticDrawMeshes;

    std::queue<std::function<void(GIImmediateCommands*)>> _refreshTasks;

public: // fixme
    std::unique_ptr<SpaceOctree::HashedOctreeFactory> _staticMeshesOctree;
    GITextureProxy _lightMap;

    // debug info
    std::vector<Line> rayPaths;
};

