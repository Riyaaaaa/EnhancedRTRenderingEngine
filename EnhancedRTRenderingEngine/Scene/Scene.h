#pragma once

#include "MeshObject.h"
#include "SceneObject.h"
#include "CameraObject.h"

#include <DirectXMath.h>
#include <vector>

class Scene
{
public:
	typedef Vertex3D VertType;

	Scene();

	DirectX::XMMATRIX GetPerspectiveProjection();
	DirectX::XMMATRIX GetWorldProjection();
	DirectX::XMMATRIX GetViewProjection();

	std::vector<MeshObject<VertType>>& GetViewObjects() { return viewObjects; }

private:
	DirectX::XMMATRIX worldProjection;
	int mainCameraIdx;

	std::vector<MeshObject<VertType>> viewObjects;
	//std::vector<SceneObject> exertObjects;
	std::vector<CameraObject> cameraObjects;

	//std::vector<SceneObject> sceneObjects;
};

