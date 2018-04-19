#include "stdafx.h"
#include "LightMapBaker.h"

#include "Mesh/MeshExpander.h"

LightMapBaker::LightMapBaker(const std::vector<MeshBase*>& bake_target, const KDimensionalTree& photonKdTree, const std::vector<Vector3D>& photons) {
    unsigned int mesh_nums = bake_target.size();
    unsigned int laid_nums = LIGHT_MAP_SIZE * LIGHT_MAP_SIZE / (mesh_nums * mesh_nums);
    
    for (unsigned int i = 0; i < mesh_nums; i++) {
        MeshExpander expander(LIGHT_MAP_SIZE / mesh_nums, 1);
        ExpandMap expanded = expander.Build(bake_target[i]);

        for (unsigned int idx = 0; idx < LIGHT_MAP_SIZE * LIGHT_MAP_SIZE; idx++) {
            auto sampledPhotons = photonKdTree.FindNeighborNNodes(expanded[idx], 100); // sampling 100 photons;
        }

        
    }
}