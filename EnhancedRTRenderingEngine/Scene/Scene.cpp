
#include "Scene.h"

#include "Common/Common.h"

#include "Algorithms/SpaceSplits.h"

#include "Utility/SceneUtils.h"
#include "Utility/TextureUtils.h"

#include "Mesh/Primitive/Primitives.h"
#include "Mesh/SimpleModel/Box.h"
#include "Mesh/Mesh3DModel.h"

#include "StaticMeshObject.h"
#include "Scene/SkyBox.h"
#include "Scene/Enviroment/StaticCubeReflectionCapture.h"
#include "Scene/Billboard.h"

#include "Material/Material.h"

#include "Resource/ResourceLoader.h"

using namespace DirectX;

Scene::Scene() {
    
}

void Scene::CreateSkyTestScene() {
    auto camera = CameraObject();
    camera.SetProjParams(D3DXToRadian(45.0f), 16.0f / 9.0f, 1.0f, 1000.0f);
    cameraObjects.push_back(camera);
    _controller = std::make_unique<CameraController>(&cameraObjects[mainCameraIdx]);

    Material material(MaterialParameters{ "LightingVertexShader", "LightingPSTextureColor", "test.png", Vector3D(), 0.2f, 0.0f });
    std::vector<Material> materials;
    materials.emplace_back(std::move(material));

    auto model2 = ResourceLoader::LoadDXModel("sphere");
    auto model = ResourceLoader::LoadDXModel("coin");

    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model()));
    viewObjects.back()->SetLocation(Vector3D{ -2.0f, 1.0f, -2.0f });
    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model()));
    viewObjects.back()->SetLocation(Vector3D{ 3.0f, 5.0f, -1.0f });
    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model()));
    viewObjects.back()->SetLocation(Vector3D{ -4.0f, 1.0f, 4.0f });
    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model()));
    viewObjects.back()->SetLocation(Vector3D{ -0.3f, 1.0f, 2.0f });

    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model2()));
    viewObjects.back()->SetLocation(Vector3D{ 5.0f, 10.0f, 5.0f });
    viewObjects.back()->SetScale(Vector3D{ 30.0f, 30.0f, 30.0f });

    viewObjects.push_back(SceneUtils::CreatePrimitiveMeshObject<PrimitiveMesh::Square<MainVertex>>(Size2D(1, 1)));
    viewObjects.back()->SetScale(Vector3D{ 20.0f, 20.0f, 20.0f });
    viewObjects.back()->SetRotation(Vector3D{ D3DX_PI / 2.0f, 0.0f, 0.0f });
    viewObjects.back()->SetLocation(Vector3D{ 0.0f, 11.0f, -0.4f });
    viewObjects.back()->SetMaterial(materials);

    auto skybox = new SkyBox("Storforsen4");
    viewObjects.push_back(skybox);

    directionalLights.emplace_back(Vector3D{ 0.0, -1.0f, 0.1f });

    StaticMeshObject<MainVertex>(std::make_shared<Mesh3DModel>(model()), Transform());

    pointLights.emplace_back(PointLight{});
    pointLights[0].SetAttenuationRadius(1000.0f);
    pointLights[0].SetPoint(Vector3D{ 2.0, 3.0f, 0.0f });

    captureObjects.push_back(new StaticCubeReflectionCapture(skybox->GetCubeTextureResource()));
    mainCameraIdx = 0;

    meshDirty = true;
    lightDirty = true;
}

void Scene::CreateGITestScene() {
    auto camera = CameraObject();
    camera.SetProjParams(D3DXToRadian(45.0f), 16.0f / 9.0f, 1.0f, 1000.0f);
    cameraObjects.push_back(camera);
    _controller = std::make_unique<CameraController>(&cameraObjects[mainCameraIdx]);

    Material material(MaterialParameters{ "LightingVertexShader", "LightingPSTextureColor", "test.png", Vector3D(), 0.2f, 0.0f });
    std::vector<Material> materials;
    materials.emplace_back(std::move(material));

    auto model2 = ResourceLoader::LoadDXModel("sphere");
    auto model3 = ResourceLoader::LoadDXModel("cornell_box");

    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model3()));
    viewObjects.back()->SetLocation(Vector3D{ 1.0f, 15.0f, 5.0f });
    viewObjects.back()->SetScale(Vector3D{ 20.0f, 20.0f, 20.0f });

    viewObjects.push_back(SceneUtils::CreateMesh3DModelObject(model2()));
    viewObjects.back()->SetLocation(Vector3D{ 5.0f, 10.0f, 5.0f });
    viewObjects.back()->SetScale(Vector3D{ 30.0f, 30.0f, 30.0f });

    viewObjects.push_back(new Billboard("test.png"));
    viewObjects.back()->SetLocation(Vector3D{ 5.0f, 3.0f, 5.0f });

    pointLights.emplace_back(PointLight{});
    pointLights[0].SetAttenuationRadius(1000.0f);
    pointLights[0].SetPoint(Vector3D{ 2.0, 3.0f, 0.0f });

    mainCameraIdx = 0;

    meshDirty = true;
    lightDirty = true;
}

void Scene::Update()
{
	for (auto&& object : viewObjects) {
		object->Update(this);
	}
}

AABB Scene::GetSceneAABB() {
    std::vector<AABB> aabbs;
    for (auto && object : viewObjects) {
        aabbs.push_back(object->GetAABB());
    }

    _precomputedAABB = SpaceSplits::CalculateOptimizedAABB(aabbs);
    return _precomputedAABB;
}

Scene::~Scene() {
    for (auto && object : viewObjects) {
        delete object;
    }
    for (auto && object : captureObjects) {
        delete object;
    }
}

XMMATRIX Scene::GetPerspectiveProjection() {
    return cameraObjects[mainCameraIdx].GetPerspectiveProjection();
}

DirectX::XMMATRIX Scene::GetViewProjection() {
    return cameraObjects[mainCameraIdx].GetViewProjection();
}

DirectX::XMVECTOR Scene::GetEyePoint() {
    return cameraObjects[mainCameraIdx].hEye;
}
