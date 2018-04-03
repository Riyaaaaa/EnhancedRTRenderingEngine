#pragma once

#include <d3d11.h>
#include <unordered_map>

#include "D3D11TextureProxy.h"
#include "ComPtr.h"
#include "Scene/Scene.h"

class D3D11SceneInfo
{
public:
    ~D3D11SceneInfo();

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

