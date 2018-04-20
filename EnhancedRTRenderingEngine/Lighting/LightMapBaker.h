#pragma once

#include <vector>

#include "Algorithms/KDimensionalTree.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GITextureProxy.h"
#include "Mesh/MeshBase.h"

class LightMapBaker
{
public:
    static constexpr unsigned int LIGHT_MAP_SIZE = 256;
    GITextureProxy Bake(GIImmediateCommands* cmd,
        const std::vector<MeshBase*>& bake_target, 
        const KDimensionalTree& photonKdTree,
        const std::vector<Vector3D>& photons);
};

