#include "stdafx.h"
#include "SceneUtils.h"

MeshObject<typename Mesh3DModel::Type> SceneUtils::CreateMesh3DModelObject(const PMDModel& model) {
    RenderingContext context{ CreateVertexLayout < typename Mesh3DModel::Type >(), VertexPrimitiveType::TRIANGLELIST };

    auto mesh3D = std::make_shared<Mesh3DModel>(model);
    MeshObject<typename Mesh3DModel::Type> mesh(mesh3D, context);
    mesh.SetMaterial(mesh3D->CreatePMDDefaultMaterials());
    
    return mesh;
}