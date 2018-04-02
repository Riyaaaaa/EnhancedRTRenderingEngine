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
    D3D11TextureProxy& GetDirectionalShadow(int index) {
        return _directionalShadows[index];
    }
    D3D11TextureProxy& GetPointShadow(int index) {
        return _pointShadows[index];
    }
    D3D11TextureProxy& GetEnviromentMap(std::size_t index) {
        return _enviromentMaps.at(index);
    }
protected:
    Scene * _scene;
    std::vector<D3D11TextureProxy> _directionalShadows;
    std::vector<D3D11TextureProxy> _pointShadows;
    std::unordered_map<std::size_t, D3D11TextureProxy> _enviromentMaps;
};

