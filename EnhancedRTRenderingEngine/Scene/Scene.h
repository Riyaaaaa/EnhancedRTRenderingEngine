#pragma once

#include "MeshObject.h"
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

	std::vector<MeshObject<Vertex3D>>& GetViewObjects() { return viewObjects; }

private:
	DirectX::XMMATRIX worldProjection;
	int mainCameraIdx;

	std::vector<MeshObject<Vertex3D>> viewObjects;
	//std::vector<SceneObject> exertObjects;
	std::vector<CameraObject> cameraObjects;

	//std::vector<SceneObject> sceneObjects;
};

