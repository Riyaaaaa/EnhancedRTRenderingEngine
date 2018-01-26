#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Square : public MeshBase<SimpleVertex>
{
public:
	Square();
	~Square();
};
