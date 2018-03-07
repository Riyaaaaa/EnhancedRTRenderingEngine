#pragma once

#include "Scene/MeshObject.h"
#include "Structure/Structure.h"

#include "Mesh/Mesh3DModel.h"
#include "Resource/PMD/PMDModel.h"

#include <vector>

namespace SceneUtils {
    template<class PrimitiveType, typename... Args>
    MeshObject<typename PrimitiveType::Type> CreatePrimitiveMeshObject(Args&&... args) {
        RenderingContext context{ CreateVertexLayout < typename PrimitiveType::Type > (), VertexPrimitiveType::TRIANGLESTRIP };
        MeshObject<PrimitiveType::Type> mesh(std::make_shared<PrimitiveType>(args...), context);
        return mesh;
    }

    MeshObject<typename Mesh3DModel::Type> CreateMesh3DModelObject(const PMDModel& model);
    MeshObject<typename Mesh3DModel::Type> CreateMesh3DModelObject(const DXModel& model);
}
