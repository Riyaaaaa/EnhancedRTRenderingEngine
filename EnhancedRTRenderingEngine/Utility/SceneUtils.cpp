#include "stdafx.h"
#include "SceneUtils.h"

MeshObject<typename Mesh3DModel::Type> SceneUtils::CreateMesh3DModelObject(const PMDModel& model) {
    RenderingContext context{ CreateVertexLayout < typename Mesh3DModel::Type >(), VertexPrimitiveType::TRIANGLELIST };
    MeshObject<typename Mesh3DModel::Type> mesh(Mesh3DModel(model), context);
    return mesh;
}