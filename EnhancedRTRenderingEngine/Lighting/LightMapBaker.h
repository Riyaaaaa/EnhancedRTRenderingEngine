#pragma once

#include <vector>

#include "Algorithms/KDimensionalTree.h"
#include "GraphicsInterface/GIImmediateCommands.h"
#include "GraphicsInterface/GITextureProxy.h"
#include "Scene/MeshObject.h"
#include "Structure/Photon.h"

class LightMapBaker
{
public:
    static constexpr unsigned int LIGHT_MAP_SIZE = 1024;
    GITextureProxy Bake(GIImmediateCommands* cmd,
        const std::vector<MeshObject<MainVertex>*>& bake_targets, 
        const KDimensionalTree<Photon>& photonKdTree,
        const std::vector<Photon>& photons);
};

