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
    MeshObject(MeshBase<VertType> mesh, RenderingContext context) : _mesh(mesh), _context(context) {}
    ~MeshObject() {}

    const MeshBase<VertType>& GetMesh() const { return _mesh; }
    const RenderingContext& GetContext() const { return _context;  }

    void SetMaterial(const Material& material) { _material = material; }
    const Material& GetMaterial() const { return _material; }

protected:
    Material _material;
    RenderingContext _context;
    MeshBase<VertType> _mesh;
};

