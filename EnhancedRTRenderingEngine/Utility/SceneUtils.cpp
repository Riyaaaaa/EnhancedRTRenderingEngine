#include "stdafx.h"

#include "SceneUtils.h"

using namespace DirectX;

ConstantBuffer SceneUtils::CreateBasePassConstantBuffer(Scene* scene) {
    ConstantBuffer hConstantBuffer;
    hConstantBuffer.View = XMMatrixTranspose(scene->GetViewProjection());
    hConstantBuffer.Projection = XMMatrixTranspose(scene->GetPerspectiveProjection());
    hConstantBuffer.Eye = scene->GetEyePoint();

    // Only support one light.
    hConstantBuffer.numDirecitonalLights = static_cast<float>(scene->GetDirectionalLights().size());
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numDirecitonalLights) {
            break;
        }
        auto& dLight = scene->GetDirectionalLights()[i];
        hConstantBuffer.DirectionalLightView[i] = XMMatrixTranspose(dLight.GetViewProjection());
        hConstantBuffer.DirectionalLightProjection[i] = XMMatrixTranspose(dLight.GetPerspectiveProjection());
        hConstantBuffer.DirectionalLight[i] = dLight.GetDirection();
    }

    hConstantBuffer.numPointLights = static_cast<float>(scene->GetPointLights().size());
    for (int i = 0; i < LIGHT_MAX; i++) {
        if (i >= hConstantBuffer.numPointLights) {
            break;
        }
        auto& pLight = scene->GetPointLights()[i];
        hConstantBuffer.PointLightProjection[i] = pLight.GetShadowPerspectiveMatrix();
        memcpy(hConstantBuffer.PointLightView[i], pLight.GetViewMatrixes(), sizeof(XMMATRIX) * 6);
        auto& pos = pLight.GetPoint();
        hConstantBuffer.PointLight[i] = PointLightParameters{ Vector4D(pos.x, pos.y, pos.z, pLight.InvSquareRadius()), Vector4D(pLight.Intensity(), 0.0, 0.0, 0.0) };
    }

    return hConstantBuffer;
}

MeshObject<typename Mesh3DModel::Type>* SceneUtils::CreateMesh3DModelObject(const PMDModel& model) {
    auto mesh3D = std::make_shared<Mesh3DModel>(model);
    MeshObject<typename Mesh3DModel::Type>* mesh = new MeshObject<typename Mesh3DModel::Type>(mesh3D);
    mesh->SetMaterial(mesh3D->CreatePMDDefaultMaterials());
    
    return mesh;
}

MeshObject<typename Mesh3DModel::Type>* SceneUtils::CreateMesh3DModelObject(const DXModel& model) {
    auto mesh3D = std::make_shared<Mesh3DModel>(model);
    MeshObject<typename Mesh3DModel::Type>* mesh = new MeshObject<typename Mesh3DModel::Type>(mesh3D);

    std::vector<Material> materials(model.mesh.meshMaterialList.nMaterials, Material::Default);
    auto& material_data = model.mesh.meshMaterialList;

    for (int i = 0; i < material_data.nMaterials; i++) {
        materials[i].baseColor = material_data.materials[i].faceColor.Slice<3>();
        materials[i].specular = material_data.materials[i].specularColor;
    }

    mesh->SetMaterial(materials);

    return mesh;
}
