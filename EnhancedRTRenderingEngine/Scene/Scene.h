#pragma once

#include "MeshObject.h"
#include "SceneObject.h"
#include "CameraObject.h"
#include "Enviroment/CubeReflectionCapture.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "CameraController.h"

#include <DirectXMath.h>
#include <vector>

class Scene
{
public:
    typedef PMDVertex VertType;

    Scene();
    ~Scene();

    bool Dirty() { return lightDirty | meshDirty; }
    bool LightDirty() { return lightDirty; }
    bool MeshDirty() { return meshDirty; }

    void SetLightDirty(bool b) { lightDirty = b; }
    void SetMeshDirty(bool b) { meshDirty = b; }

    DirectX::XMMATRIX GetPerspectiveProjection();
    DirectX::XMMATRIX GetWorldProjection();
    DirectX::XMMATRIX GetViewProjection();

    std::vector<MeshObject<VertType>>& GetViewObjects() { return viewObjects; }
    std::vector<DirectionalLight>& GetDirectionalLights() { return directionalLights; }

    std::vector<PointLight>& GetPointLights() { return pointLights; }
    std::vector<PointLightParameters> GetPointLightParams();
    std::vector<CubeReflectionCapture*> GetReflectionCaptures() { return captureObjects; }

    DirectX::XMVECTOR GetEyePoint();

private:
    bool lightDirty;
    bool meshDirty;

    DirectX::XMMATRIX worldProjection;
    int mainCameraIdx;

    std::vector<MeshObject<VertType>> viewObjects;
    //std::vector<SceneObject> exertObjects;
    std::vector<CameraObject> cameraObjects;
    std::vector<CubeReflectionCapture*> captureObjects;

    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;

    std::unique_ptr<CameraController> _controller;

    //std::vector<SceneObject> sceneObjects;
};

