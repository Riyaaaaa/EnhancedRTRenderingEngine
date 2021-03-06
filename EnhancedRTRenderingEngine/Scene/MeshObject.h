#pragma once

#include "SceneObject.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "Material/Material.h"
#include "RenderingContext.h"
#include "Scene/Enviroment/CubeReflectionCapture.h"

#include "Structure/RaytraceStructures.h"
#include "Mesh/StaticLightBuildData.h"

#include <memory>

class MeshObjectBase : public SceneObject {
public:
    virtual AABB GetAABB() = 0;
    virtual Vector3D GetVertexPosition(unsigned int idx) = 0;
    virtual unsigned int GetVertexNums() = 0;
    virtual std::vector<Hit> IntersectPositions(Ray ray) = 0;

    void SetMaterial(const std::vector<Material>& materials) {
        _materials = materials;
    }
    void SetMaterial(std::vector<Material>&& materials) {
        // assert(_mesh->GetDrawElementMap().size() == materials.size);
        _materials.swap(materials);
    }

    const std::vector<Material>& GetMaterials() const { return _materials; }

    void FindPrecisionReflectionSource(const std::vector<CubeReflectionCapture*>& captures);
    bool HasReflectionSource() { return _hasReflectionSource; }
    std::size_t GetReflectionSourceId() { return _reflectionSourceId; }

    bool HasLightMap() const { return _hasLightMap; }
    virtual void SetLightMap(const StaticLightBuildData& map) { 
        _hasLightMap = true;
        _lightMapData = map; 
    }
    const StaticLightBuildData& GetLightBuildData() const {
        return _lightMapData;
    }

    bool IsPhysicalObject() {
        return _isPhysicallyObject;
    }

    bool IsStatic() {
        return true;
    }

protected:
    virtual void DirtyWorldMatrix() override;

    MeshObjectBase() = default;

    bool AABBDirty = true;
    bool _hasLightMap = false;
    bool _hasReflectionSource = false;
    bool _isPhysicallyObject = true;

    std::vector<DirectX::XMVECTOR> _vertexTransformedCache;

    std::size_t _reflectionSourceId;
    StaticLightBuildData _lightMapData;
    std::vector<Material> _materials;
};

template<class VertType>
class MeshObject : public MeshObjectBase
{
public:
    MeshObject(const std::shared_ptr<Mesh<VertType>>& mesh) : _mesh(mesh) {
        _materials.resize(_mesh->ElementSize(), Material::Default);
    }

    const std::shared_ptr<Mesh<VertType>>& GetMesh() const { return _mesh; }

    std::vector<Triangle> GetTransformedTriangles();

    virtual AABB GetAABB() override;
    virtual Vector3D GetVertexPosition(unsigned int idx) override;
    virtual unsigned int GetVertexNums() override;
    virtual std::vector<Hit> IntersectPositions(Ray ray) override;

protected:
    std::shared_ptr<Mesh<VertType>> _mesh;
    MeshObject() = default;
};

