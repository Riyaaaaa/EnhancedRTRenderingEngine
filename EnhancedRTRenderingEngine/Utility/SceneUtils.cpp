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
        hConstantBuffer.PointLight[i] = PointLightParameters{ pLight.GetPoint(), pLight.GetAttenuation() };
    }

    return hConstantBuffer;
}

MeshObject<typename Mesh3DModel::Type> SceneUtils::CreateMesh3DModelObject(const PMDModel& model) {
    auto mesh3D = std::make_shared<Mesh3DModel>(model);
    MeshObject<typename Mesh3DModel::Type> mesh(mesh3D);
    mesh.SetMaterial(mesh3D->CreatePMDDefaultMaterials());
    
    return mesh;
}

MeshObject<typename Mesh3DModel::Type> SceneUtils::CreateMesh3DModelObject(const DXModel& model) {
    auto mesh3D = std::make_shared<Mesh3DModel>(model);
    MeshObject<typename Mesh3DModel::Type> mesh(mesh3D);
    mesh.SetMaterial(mesh3D->CreatePMDDefaultMaterials());

    return mesh;
}
