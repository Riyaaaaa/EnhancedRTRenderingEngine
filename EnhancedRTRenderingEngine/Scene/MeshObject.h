#pragma once

#include "SceneObject.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "Material/Material.h"
#include "RenderingContext.h"

#include <memory>

template<class VertType>
class MeshObject : public SceneObject
{
public:
    MeshObject(const std::shared_ptr<MeshBase<VertType>>& mesh, RenderingContext context) : _mesh(mesh), _context(context) {}

    const std::shared_ptr<MeshBase<VertType>>& GetMesh() const { return _mesh; }
    const RenderingContext& GetContext() const { return _context;  }

    void SetMaterial(const std::vector<Material>& materials) { _materials = materials; }
    const std::vector<Material>& GetMaterials() const { return _materials; }

protected:
    std::vector<Material> _materials;
    RenderingContext _context;
    std::shared_ptr<MeshBase<VertType>> _mesh;
};

