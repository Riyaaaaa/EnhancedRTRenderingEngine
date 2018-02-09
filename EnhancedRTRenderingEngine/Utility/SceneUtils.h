#pragma once

#include "Scene/MeshObject.h"
#include "Structure/Structure.h"

#include "Mesh/Mesh3DModel.h"
#include "Resource/PMD/PMDModel.h"

#include <vector>

namespace SceneUtils {
    template<class PrimitiveType>
    MeshObject<typename PrimitiveType::Type> CreatePrimitiveMeshObject() {
        RenderingContext context{ CreateVertexLayout < typename PrimitiveType::Type > (), VertexPrimitiveType::TRIANGLESTRIP };
        MeshObject<PrimitiveType::Type> mesh(new PrimitiveType{}, context);
        return mesh;
    }

    MeshObject<typename Mesh3DModel::Type> CreateMesh3DModelObject(const PMDModel& model);
}
