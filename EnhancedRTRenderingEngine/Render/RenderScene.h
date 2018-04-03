#pragma once

#include <unordered_map>

#include "GraphicsInterface/GITextureProxy.h"
#include "Scene/Scene.h"

class RenderScene
{
public:
    ~RenderScene();

    void Refresh(GIImmediateCommands* cmd, Scene* scene);
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
protected:
    Scene * _scene;
    std::vector<GITextureProxy> _directionalShadows;
    std::vector<GITextureProxy> _pointShadows;
    std::unordered_map<std::size_t, GITextureProxy> _enviromentMaps;
};

