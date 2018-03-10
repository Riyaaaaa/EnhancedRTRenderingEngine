#pragma once

#include <unordered_map>

#include "Scene/Scene.h"
#include "D3D11DepthStencilTarget.h"

class D3D11SceneInfo
{
public:
    ~D3D11SceneInfo();

    void Refresh(ComPtr<ID3D11Device> device, Scene* scene);
    Scene* GetSourceScene() { return _scene; }
    D3D11Texture& GetDirectionalShadow(int index) {
        return _directionalShadows[index];
    }
    D3D11Texture& GetPointShadow(int index) {
        return _pointShadows[index];
    }
    D3D11Texture& GetEnviromentMap(std::size_t index) {
        return _enviromentMaps.at(index);
    }
protected:
    Scene * _scene;
    std::vector<D3D11Texture> _directionalShadows;
    std::vector<D3D11Texture> _pointShadows;
    std::unordered_map<std::size_t, D3D11Texture> _enviromentMaps;
};

