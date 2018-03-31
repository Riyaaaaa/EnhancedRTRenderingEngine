#pragma once

#include "MeshObject.h"
//#include "StaticMeshObject.h"
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

    void CreateTestScene();

    bool Dirty() { return lightDirty | meshDirty; }
    bool LightDirty() { return lightDirty; }
    bool MeshDirty() { return meshDirty; }

    void SetLightDirty(bool b) { lightDirty = b; }
    void SetMeshDirty(bool b) { meshDirty = b; }

    DirectX::XMMATRIX GetPerspectiveProjection();
    DirectX::XMMATRIX GetViewProjection();

    std::vector<MeshObject<VertType>>& GetViewObjects() { return viewObjects; }
    std::vector<DirectionalLight>& GetDirectionalLights() { return directionalLights; }

    std::vector<PointLight>& GetPointLights() { return pointLights; }

    std::vector<CubeReflectionCapture*> GetReflectionCaptures() { return captureObjects; }

    DirectX::XMVECTOR GetEyePoint();

    AABB GetSceneAABB();

    void SetMainCamera(const CameraObject& camera) {
        cameraObjects.push_back(camera);
        mainCameraIdx = cameraObjects.size() - 1;
    }

    CameraObject GetMainCamera() const {
        return cameraObjects[mainCameraIdx];
    }

private:
    bool lightDirty;
    bool meshDirty;

    DirectX::XMMATRIX worldProjection;
    int mainCameraIdx;

    AABB _precomputedAABB;

    std::vector<MeshObject<VertType>> viewObjects;
    std::vector<CameraObject> cameraObjects;
    std::vector<CubeReflectionCapture*> captureObjects;
    std::vector<DirectionalLight> directionalLights;
    std::vector<PointLight> pointLights;

    std::unique_ptr<CameraController> _controller;

    //std::vector<SceneObject> sceneObjects;
};

