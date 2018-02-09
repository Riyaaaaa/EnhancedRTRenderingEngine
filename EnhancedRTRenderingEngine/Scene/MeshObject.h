#pragma once

#include "SceneObject.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "Material/Material.h"
#include "RenderingContext.h"

template<class VertType>
class MeshObject : public SceneObject
{
public:
    MeshObject(MeshBase<VertType>* mesh, RenderingContext context) : _mesh(mesh), _context(context) {}
    ~MeshObject() {
        delete _mesh;
    }

    const MeshBase<VertType>* GetMesh() const { return _mesh; }
    const RenderingContext& GetContext() const { return _context;  }

    void SetMaterial(const std::vector<Material>& materials) { _materials = materials; }
    const std::vector<Material>& GetMaterials() const { return _materials; }

protected:
    std::vector<Material> _materials;
    RenderingContext _context;
    MeshBase<VertType>* _mesh;
};

