#pragma once

#include "MeshObject.h"
#include "SceneObject.h"
#include "CameraObject.h"
#include "PointLight.h"
#include "DirectionalLight.h"

#include <DirectXMath.h>
#include <vector>

class Scene
{
public:
	typedef PMDVertex VertType;

	Scene();

	DirectX::XMMATRIX GetPerspectiveProjection();
	DirectX::XMMATRIX GetWorldProjection();
	DirectX::XMMATRIX GetViewProjection();

	std::vector<MeshObject<VertType>>& GetViewObjects() { return viewObjects; }
	std::vector<DirectionalLight>& GetDirectionalLights() { return directionalLights; }

	std::vector<PointLightParameters> GetPointLightParams();

private:
	DirectX::XMMATRIX worldProjection;
	int mainCameraIdx;

	std::vector<MeshObject<VertType>> viewObjects;
	//std::vector<SceneObject> exertObjects;
	std::vector<CameraObject> cameraObjects;

	std::vector<DirectionalLight> directionalLights;
	std::vector<PointLight> pointLights;

	//std::vector<SceneObject> sceneObjects;
};

