#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Square : public MeshBase<Vertex3D>
{
public:
	Square();
};

class SquarePMD : public MeshBase<PMDVertex>
{
public:
	SquarePMD();
};
