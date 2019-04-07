#pragma once

#include "MeshObject.h"
#include "Structure/Structure.h"

class Billboard : public MeshObject<MainVertex>
{
public:
	Billboard(const std::string& texture);
};
