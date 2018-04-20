#include "stdafx.h"
#include "LightMapBaker.h"

#include "Mesh/MeshExpander.h"

LightMapBaker::LightMapBaker(const std::vector<MeshBase*>& bake_target, const KDimensionalTree& photonKdTree, const std::vector<Vector3D>& photons) {
    unsigned int mesh_nums = bake_target.size();
    unsigned int laid_nums = LIGHT_MAP_SIZE * LIGHT_MAP_SIZE / (mesh_nums * mesh_nums);
    
    for (unsigned int i = 0; i < mesh_nums; i++) {
        StaticLightBuildData light_build_data;
        unsigned int local_map_size = LIGHT_MAP_SIZE / mesh_nums;
        MeshExpander expander(local_map_size, 1);
        ExpandMap expanded = expander.Build(bake_target[i]);

        for (unsigned int idx = 0; idx < LIGHT_MAP_SIZE * LIGHT_MAP_SIZE; idx++) {
            auto sampledPhotons = photonKdTree.FindNeighborNNodes(expanded(idx).worldPosition.Slice<3>(), 100); // sampling 100 photons;
        }

        unsigned int nIndices = expanded.TriangleBaseIndices().size();
        light_build_data.lightMapUVs.resize(nIndices);
        for (unsigned int idx = 0; idx < nIndices; idx++) {
            auto local_pos = expanded.TriangleBaseIndices()[idx];
            auto global_pos = _Vector2D<unsigned int>(
                (i % laid_nums) * local_map_size + local_pos.x,
                (i / laid_nums) * local_map_size + local_pos.y);

            Vector2D uv(global_pos.x / (float)LIGHT_MAP_SIZE, global_pos.y / (float)LIGHT_MAP_SIZE);
            light_build_data.lightMapUVs[idx] = uv;
        }
    }
}