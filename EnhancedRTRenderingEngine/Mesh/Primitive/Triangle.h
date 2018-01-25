#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Triangle : public MeshBase<Vertex3D>
{
public:
	Triangle();
	~Triangle();
};

