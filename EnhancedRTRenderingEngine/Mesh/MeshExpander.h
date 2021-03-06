#pragma once

#include "Structure/Vector.h"
#include "Structure/Structure.h"
#include "Structure/Primitive.h"

#include "MeshBase.h"
#include "StaticLightBuildData.h"

class MeshExpander
{
public:
    MeshExpander(unsigned int size, unsigned int margin = 1);

    ExpandMap Build(const std::vector<Triangle>& mesh_data);

protected:
    unsigned int  _expandSize, _margin;
};

