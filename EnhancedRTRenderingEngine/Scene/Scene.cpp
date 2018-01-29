#include "stdafx.h"
#include "Scene.h"

#include "Common.h"

#include "Utility/SceneUtils.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"

#include "Material/Material.h"

using namespace DirectX;

Scene::Scene() {
	worldProjection = XMMatrixIdentity();

	XMMATRIX hRotate;
	// test code
	hRotate = XMMatrixRotationZ(D3DXToRadian(-45.0f));
	worldProjection = XMMatrixMultiply(worldProjection, hRotate);
	hRotate = XMMatrixRotationY(D3DXToRadian(-45.0f));
	worldProjection = XMMatrixMultiply(worldProjection, hRotate);

	auto camera = CameraObject();
	camera.SetProjParams(D3DXToRadian(45.0f), 16.0f / 9.0f, 0.0001f, 1000.0f);
	cameraObjects.push_back(camera);

	//test code
	//viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<Square>());
	viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<Box>());

	Material material(MaterialParameters{"VertexShader", "PixelShader", "test"});
	viewObjects[0].SetMaterial(material);

	mainCameraIdx = 0;
}

XMMATRIX Scene::GetPerspectiveProjection() {
	return cameraObjects[mainCameraIdx].GetPerspectiveProjection();
}

XMMATRIX Scene::GetWorldProjection() {
	return worldProjection;
}

DirectX::XMMATRIX Scene::GetViewProjection() {
	return cameraObjects[mainCameraIdx].GetViewProjection();
}
