#pragma once

#include <unordered_map>

#include "Algorithms/SpaceOctree.h"
#include "GraphicsInterface/GITextureProxy.h"
#include "Scene/Scene.h"

#include "DrawMesh.h"

class RenderScene
{
public:
    RenderScene(Scene* scene) : _scene(scene) {}
    ~RenderScene();

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
    std::unordered_map<std::size_t, DrawMesh>& GetStaticDrawMeshes() {
        return _staticDrawMeshes;
    }
    std::vector<DrawElement>& GetDrawList() {
        return _drawList;
    }
protected:
    Scene * _scene;
    std::vector<GITextureProxy> _directionalShadows;
    std::vector<GITextureProxy> _pointShadows;
    std::unordered_map<std::size_t, GITextureProxy> _enviromentMaps;

    std::vector<DrawElement> _drawList;
    std::unordered_map<std::size_t, DrawMesh> _staticDrawMeshes;

public: // fixme
    std::unique_ptr<SpaceOctree::HashedOctreeFactory> _staticMeshesOctree;
    GITextureProxy _lightMap;
};

