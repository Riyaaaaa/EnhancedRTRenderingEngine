#pragma once

#include "SceneObject.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "Material/Material.h"
#include "RenderingContext.h"
#include "Scene/Enviroment/CubeReflectionCapture.h"

#include <memory>

class IMeshObject {
public:
    // add if necessary
    virtual AABB GetAABB() = 0;
};

template<class VertType>
class MeshObject : public SceneObject, public IMeshObject
{
public:
    MeshObject(const std::shared_ptr<MeshBase<VertType>>& mesh) : _mesh(mesh) {}

    const std::shared_ptr<MeshBase<VertType>>& GetMesh() const { return _mesh; }

    void SetMaterial(const std::vector<Material>& materials) {
        _materials = materials;
    }
    void SetMaterial(std::vector<Material>&& materials) {
        _materials.swap(materials);
    }

    const std::vector<Material>& GetMaterials() const { return _materials; }

    void FindPrecisionReflectionSource(const std::vector<CubeReflectionCapture*>& captures);
    bool HasReflectionSource() { return _hasReflectionSource; }
    std::size_t GetReflectionSourceId() { return _reflectionSourceId; }

    virtual AABB GetAABB() override;

protected:
    virtual void DirtyWorldMatrix() override;

    MeshObject() = default;
    bool AABBDirty = true;
    bool _hasReflectionSource = false;
    std::size_t _reflectionSourceId;
    std::vector<Material> _materials;
    std::shared_ptr<MeshBase<VertType>> _mesh;
};

template<class VertType>
void MeshObject<VertType>::FindPrecisionReflectionSource(const std::vector<CubeReflectionCapture*>& captures) {
    float precision = FLT_MAX;
    
    _hasReflectionSource = false;
    for (auto&& capture: captures) {
        if (capture->Contains(_transform.location) && capture->PrecisionSize() < precision) {
            precision = capture->PrecisionSize();
            _reflectionSourceId = capture->GetID();
            _hasReflectionSource = true;
        }
    }
}
