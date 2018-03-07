#pragma once

#include "Scene/Scene.h"
#include "D3D11DepthStencilTarget.h"

class D3D11Scene
{
public:
    D3D11Scene();
    ~D3D11Scene();

    void Refresh(Scene* scene);
    Scene* GetSourceScene() { return _scene; }
    D3D11Texture& GetDirectionalShadow(int index) {
        return _directionalShadows[index];
    }
    D3D11Texture& GetPointShadow(int index) {
        return _pointShadows[index];
    }
protected:
    Scene * _scene;
    std::vector<D3D11Texture> _directionalShadows;
    std::vector<D3D11Texture> _pointShadows;
};

