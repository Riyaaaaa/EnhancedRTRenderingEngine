#include "stdafx.h"
#include "Scene.h"

#include "Common.h"

#include "Utility/SceneUtils.h"
#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"
#include "Mesh/Mesh3DModel.h"

#include "Material/Material.h"

#include "Resource/ResourceLoader.h"

using namespace DirectX;

Scene::Scene() {
	worldProjection = XMMatrixIdentity();

	XMMATRIX hRotate;
	// test code
	hRotate = XMMatrixRotationX(D3DXToRadian(45.0f));
	worldProjection = XMMatrixMultiply(worldProjection, hRotate);
	/*
	hRotate = XMMatrixRotationY(D3DXToRadian(-45.0f));
	worldProjection = XMMatrixMultiply(worldProjection, hRotate);*/

	auto camera = CameraObject();
	camera.SetProjParams(D3DXToRadian(45.0f), 16.0f / 9.0f, 0.0001f, 1000.0f);
	cameraObjects.push_back(camera);

	//test code
	//viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<Square>());
	//viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<Box>());

	//viewObjects[0].SetLocation(Vector3D{ -1.0f, 0.0f, 0.0f });
	//viewObjects[1].SetLocation(Vector3D{ +1.0f, 0.0f, 0.0f });

	Material material(MaterialParameters{"LightingVertexShader", "LightingPixelShader", "test3"});

	auto model = ResourceLoader::LoadPMDModel("nolicensed");
	viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model()));
	
	viewObjects[0].SetMaterial(material);
	//viewObjects[1].SetMaterial(material);

	directionalLights.push_back(DirectionalLight(Vector3D{0.0, 0.5f, -1.0f}));

	PointLight plight;
	plight.SetAttenuation(Vector3D{ 1.0f, 0.1f, 0.01f });
	plight.SetPoint(Vector3D{ 0.0, -1.0f, 0.0f });

	pointLights.push_back(plight);

	mainCameraIdx = 0;

	_controller = std::make_unique<CameraController>(&cameraObjects[mainCameraIdx]);
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

std::vector<PointLightParameters> Scene::GetPointLightParams() {
	std::vector<PointLightParameters> params;

	for (auto && light : pointLights) {
		params.push_back(PointLightParameters{light.GetPoint(), light.GetAttenuation()});
	}

	return params;
}
