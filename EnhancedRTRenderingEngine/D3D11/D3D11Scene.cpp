#include "stdafx.h"
#include "D3D11Scene.h"

#include "Constant/RenderConfig.h"

D3D11Scene::D3D11Scene() : 
    _directionalShadows(LIGHT_MAX),
    _pointShadows(LIGHT_MAX)
{
}


D3D11Scene::~D3D11Scene()
{
}

void D3D11Scene::Refresh(Scene* scene) {
    _scene = scene;
}
