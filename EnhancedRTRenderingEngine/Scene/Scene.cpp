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
	viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<SquarePMD>());

	viewObjects[0].SetLocation(Vector3D{ 0.0f, 0.0f, 0.0f });
	viewObjects[1].SetScale(Vector3D{ 20.0f, 20.0f, 20.0f });
	viewObjects[1].SetRotation(Vector3D{ D3DX_PI / 2.0f, 0.0f, 0.0f });
	viewObjects[1].SetLocation(Vector3D{ 0.0f, 0.0f, -0.4f });
	viewObjects[0].SetMaterial(material);
	viewObjects[1].SetMaterial(material);

	//directionalLights.push_back(DirectionalLight(Vector3D{0.0, 0.0f, -1.0f}));

	PointLight plight;
	plight.SetAttenuation(Vector3D{ 1.0f, 0.1f, 0.01f });
	plight.SetPoint(Vector3D{ 0.0, 3.0f, -3.0f });

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

DirectX::XMMATRIX Scene::GetDirectionalLightViewProjection() {
	auto& dLights = GetDirectionalLights();
	if (dLights.empty()) {
		return DirectX::XMMATRIX();
	}
	auto lDir = dLights[0].GetDirection();
	DirectX::XMVECTOR pos = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR dir = XMVectorSet(lDir.x, lDir.y, lDir.z, 0.0f);
	DirectX::XMVECTOR hUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static const DirectX::XMMATRIX SHADOW_BIAS = DirectX::XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f);

	return XMMatrixMultiply(XMMatrixLookToLH(pos, dir, hUp), SHADOW_BIAS);
}

std::vector<PointLightParameters> Scene::GetPointLightParams() {
	std::vector<PointLightParameters> params;

	for (auto && light : pointLights) {
		params.push_back(PointLightParameters{light.GetPoint(), light.GetAttenuation()});
	}

	return params;
}
