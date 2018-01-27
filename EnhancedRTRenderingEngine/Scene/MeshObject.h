#pragma once

#include "SceneObject.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "RenderingContext.h"

template<class VertType>
class MeshObject : public SceneObject
{
public:
	MeshObject(MeshBase<VertType> mesh, RenderingContext context) : _mesh(mesh), _context(context) {}
	~MeshObject() {}

	const MeshBase<VertType>& getMesh() { return _mesh; }
	const RenderingContext& GetContext() { return _context;  }

protected:
	RenderingContext _context;
	MeshBase<VertType> _mesh;
};

