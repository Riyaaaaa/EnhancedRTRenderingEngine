#include "stdafx.h"
#include "Scene.h"

#include "Common.h"

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
