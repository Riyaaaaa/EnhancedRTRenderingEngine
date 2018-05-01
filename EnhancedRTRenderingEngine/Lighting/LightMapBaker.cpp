#include "stdafx.h"
#include "LightMapBaker.h"

#include "Common/Common.h"
#include "Mesh/MeshExpander.h"
#include "Structure/TextureBuffer.h"
#include "Utility/MathUtils.h"

#include "Common/Defines.h"

GITextureProxy LightMapBaker::Bake(GIImmediateCommands* cmd, const std::vector<MeshObject<MainVertex>*>& bake_targets, const KDimensionalTree<Photon>& photonKdTree, const std::vector<Photon>& photons) {
    unsigned int mesh_nums = bake_targets.size();
    unsigned int local_map_size = LIGHT_MAP_SIZE / mesh_nums;

    // Now implement, square light maps, therefore x==y
    unsigned int laid_nums_x = LIGHT_MAP_SIZE / local_map_size;
    //unsigned int laid_nums_y = LIGHT_MAP_SIZE / local_map_size;

    TextureBuffer buf(LIGHT_MAP_SIZE, LIGHT_MAP_SIZE);
    buf.FillColor(Vector4D(0.0f, 0.0f, 0.0f, 0.0f));

    for (unsigned int i = 0; i < mesh_nums; i++) {
        auto* bake_target = bake_targets[i];
        StaticLightBuildData light_build_data;
        
        MeshExpander expander(local_map_size, 1);
        ExpandMap expanded = expander.Build(bake_target->GetTransformedTriangles());

        unsigned int nIndices = expanded.TriangleBaseIndices().size();
        light_build_data.lightVertices.resize(nIndices);

        _Vector2D<unsigned int> base_coord(i % laid_nums_x * local_map_size, i / laid_nums_x * local_map_size);

#ifdef SUPPORT_MULTI_PROCESS_OMP
#pragma omp parallel for
        for (int idx = 0; idx < static_cast<int>(local_map_size * local_map_size); idx++) {
#elif
        for (unsigned int idx = 0; idx < local_map_size * local_map_size; idx++) {
#endif
            Vector3D raddiance = Vector3D(0.0f, 0.0f, 0.0f);
            Vector3D accumulated_flux(0.0f, 0.0f, 0.0f);
            _Vector2D<unsigned int> global_coord = base_coord + _Vector2D<unsigned int>(idx % local_map_size, idx / local_map_size);
            float triangle_idx = expanded(idx).belongsTriangleIdx;
            if (triangle_idx != -1) {
                auto sampledPhotons = photonKdTree.FindNeighborNNodes(expanded(idx).worldPosition.Slice<3>(), 10, 3.0f); // sampling 10 photons;
                if (!sampledPhotons.empty()) {
                    float r = std::sqrtf(sampledPhotons.back().second); // kd-tree find method returns array sorted by distance 
                    //float A = D3DX_PI * 3.0f * 3.0f; // FIXME: Use sampling limit radius or farthest photon distance as a radius??
                    float A = D3DX_PI * r * r;
                    const float k = 1.1f;

                    for (std::size_t photon_idx = 0; photon_idx < sampledPhotons.size(); photon_idx++) {
                        const float w = 1.0 - std::sqrtf(sampledPhotons[photon_idx].second) / (k * r);

                        accumulated_flux += photons[sampledPhotons[photon_idx].first->index].power * w / D3DX_PI; // 
                    }
                    accumulated_flux = accumulated_flux / (1.0f - 2.0f / (3.0f / k));
                    
                    if (r > 0.0f) {
                        raddiance = accumulated_flux / A;
                    }
                }
            }

            //TODO: photons transfer RGB colors
            buf(global_coord.x, global_coord.y, 0) = std::min(255, (int)(raddiance.x));
            buf(global_coord.x, global_coord.y, 1) = std::min(255, (int)(raddiance.y));
            buf(global_coord.x, global_coord.y, 2) = std::min(255, (int)(raddiance.z));
            buf(global_coord.x, global_coord.y, 3) = 255;
        }

        for (unsigned int idx = 0; idx < nIndices; idx++) {
            auto local_pos = expanded.TriangleBaseIndices()[idx];
            auto global_pos = _Vector2D<unsigned int>(
                (i % laid_nums_x) * local_map_size + local_pos.x,
                (i / laid_nums_x) * local_map_size + local_pos.y);

            Vector2D uv(global_pos.x / (float)LIGHT_MAP_SIZE, global_pos.y / (float)LIGHT_MAP_SIZE);
            MainVertex src_indices = bake_target->GetMesh()->GetVertex(idx);
            src_indices.lightUV = Vector2D(global_pos.x / (float)LIGHT_MAP_SIZE, global_pos.y / (float)LIGHT_MAP_SIZE);
            light_build_data.lightVertices[idx] = src_indices;
        }

        bake_targets[i]->SetLightMap(light_build_data);
    }

    TextureParam param;
    param.width = LIGHT_MAP_SIZE;
    param.height = LIGHT_MAP_SIZE;
    GITextureProxy lightMap = MakeRef(cmd->CreateTextureProxy(param, Texture2D(LIGHT_MAP_SIZE, LIGHT_MAP_SIZE, 4, buf.Get(), buf.Size())));

    return lightMap;
}
