#pragma once

#include "Scene/Scene.h"
#include "Scene/MeshObject.h"
#include "Structure/Structure.h"

#include "Mesh/Mesh3DModel.h"
#include "Resource/PMD/PMDModel.h"

#include <vector>

namespace SceneUtils {
    ConstantBuffer CreateBasePassConstantBuffer(Scene* scene);

    template<class PrimitiveType, typename... Args>
    MeshObject<typename PrimitiveType::Type>* CreatePrimitiveMeshObject(Args&&... args) {
        MeshObject<PrimitiveType::Type>* mesh = new MeshObject<PrimitiveType::Type>(std::make_shared<PrimitiveType>(args...));
        return mesh;
    }

    MeshObject<typename Mesh3DModel::Type>* CreateMesh3DModelObject(const PMDModel& model);
    MeshObject<typename Mesh3DModel::Type>* CreateMesh3DModelObject(const DXModel& model);
}
