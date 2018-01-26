#pragma once

#include "SceneObject.h"
#include "CameraObject.h"

#include <DirectXMath.h>
#include <vector>

class Scene
{
public:
	Scene();

	DirectX::XMMATRIX GetPerspectiveProjection();
	DirectX::XMMATRIX GetWorldProjection();
	DirectX::XMMATRIX GetViewProjection();


private:
	DirectX::XMMATRIX worldProjection;
	int mainCameraIdx;

	std::vector<SceneObject> viewObjects;
	//std::vector<SceneObject> exertObjects;
	std::vector<CameraObject> cameraObjects;

	//std::vector<SceneObject> sceneObjects;
};

