#include "stdafx.h"
#include "LightMapBaker.h"

#include "Common/Common.h"
#include "Mesh/MeshExpander.h"
#include "Structure/TextureBuffer.h"
#include "Utility/MathUtils.h"

void LightMapBaker::Bake(GIImmediateCommands* cmd, const std::vector<MeshBase*>& bake_target, const KDimensionalTree& photonKdTree, const std::vector<Vector3D>& photons) {
    unsigned int mesh_nums = bake_target.size();
    unsigned int laid_nums = LIGHT_MAP_SIZE * LIGHT_MAP_SIZE / (mesh_nums * mesh_nums);

    TextureBuffer buf(LIGHT_MAP_SIZE, LIGHT_MAP_SIZE);
    
    for (unsigned int i = 0; i < mesh_nums; i++) {
        StaticLightBuildData light_build_data;
        unsigned int local_map_size = LIGHT_MAP_SIZE / mesh_nums;
        MeshExpander expander(local_map_size, 1);
        ExpandMap expanded = expander.Build(bake_target[i]);

        _Vector2D<unsigned int> base_coord(i % LIGHT_MAP_SIZE, i / LIGHT_MAP_SIZE);

        for (unsigned int idx = 0; idx < local_map_size * local_map_size; idx++) {
            auto sampledPhotons = photonKdTree.FindNeighborNNodes(expanded(idx).worldPosition.Slice<3>(), 100); // sampling 100 photons;

            float r = std::sqrtf(sampledPhotons.back().second); // kd-tree find method returns array sorted by distance 
            float A = D3DX_PI * r * r;
            float raddiance = sampledPhotons.size() / A;

            _Vector2D<unsigned int> local_coord = base_coord + _Vector2D<unsigned int>(idx % local_map_size, idx / local_map_size);

            //TODO: photons transfer RGB colors
            buf(local_coord.x, local_coord.y, 0) = raddiance * 255;
            buf(local_coord.x, local_coord.y, 1) = raddiance * 255;
            buf(local_coord.x, local_coord.y, 2) = raddiance * 255;
            buf(local_coord.x, local_coord.y, 3) = 255;
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

    TextureParam param;
    param.width = LIGHT_MAP_SIZE;
    param.height = LIGHT_MAP_SIZE;
    lightMap = MakeRef(cmd->CreateTextureProxy(param, Texture2D(LIGHT_MAP_SIZE, LIGHT_MAP_SIZE, 4, buf.Get(), buf.Size())));
}
