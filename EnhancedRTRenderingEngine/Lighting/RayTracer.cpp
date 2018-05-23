#include "stdafx.h"

#include "Lighting/RayTracer.h"

#include "Structure/Primitive.h"
#include "Constant/PrecomputedConstants.h"

std::vector<Segment> RayTrace(SpaceOctree::OctreeFactoryBase* factory, Ray ray, unsigned int traceLevel) {
    std::vector<Segment> rayRoutes;

    for (unsigned int i = 0; i < traceLevel; i++) {
        auto clist = GetColliderMortonList(factory, ray);
        bool hitted = false;
        Hit hit;
        hit.distance = FLT_MAX;
        for (auto && idx : clist) {
            auto box = factory->GetOctreeBox(idx).get();

            auto it = box->nodes.begin();
            while (it != box->nodes.end()) {
                auto& node = *it;
                it++;

                std::vector<Hit> ret = node->object->IntersectPositions(ray);

                if (ret.empty()) {
                    continue;
                }

                Hit nearest = *std::min_element(ret.begin(), ret.end(), [](const Hit& lhs, const Hit& rhs) { return lhs.distance < rhs.distance; });

                if (hit.distance > nearest.distance) {
                    hit = nearest;
                    hitted = true;
                }
            }
        }

        if (!hitted) {
            break;
        }
        rayRoutes.push_back(Segment(ray.pos, hit.pos));
        ray.dir = hit.nextDir;
        ray.pos = hit.pos;
    }

    rayRoutes.push_back(Segment(ray.pos, ray.pos + ray.dir * 100));
    return rayRoutes;
}

std::vector<Segment> RayTraceIf(SpaceOctree::OctreeFactoryBase* factory, Ray ray, std::function<bool(const Material&, const Hit&, int)> hit_program) {
    std::vector<Segment> rayRoutes;

    Material mat;
    Hit hit;
    int trace_count = 0;
    do {
        auto clist = GetColliderMortonList(factory, ray);
        bool hitted = false;
        hit.distance = FLT_MAX;
        for (auto && idx : clist) {
            auto box = factory->GetOctreeBox(idx).get();

            auto it = box->nodes.begin();
            while (it != box->nodes.end()) {
                auto& node = *it;
                it++;

                std::vector<Hit> ret = node->object->IntersectPositions(ray);

                if (ret.empty()) {
                    continue;
                }

                Hit nearest = *std::min_element(ret.begin(), ret.end(), [](const Hit& lhs, const Hit& rhs) { return lhs.distance < rhs.distance; });

                if (hit.distance > nearest.distance) {
                    hit = nearest;
                    mat = node->object->GetMaterials()[nearest.materialIdx];
                    hitted = true;
                }
            }
        }

        if (!hitted) {
            break;
        }
        rayRoutes.push_back(Segment(ray.pos, hit.pos));
        ray.dir = hit.nextDir;
        ray.pos = hit.pos;
        trace_count++;
    } while (hit_program(mat, hit, trace_count));

    return rayRoutes;
}

std::set<uint32_t> GetColliderMortonList(SpaceOctree::OctreeFactoryBase* factory, Ray ray) {
    auto min_size = factory->GetMinBoxSize();
    auto rootAABB = factory->GetRootAABB();
    
    _Vector3D<int16_t> grid = factory->CalculateGridCoordinate(ray.pos);
    _Vector3D<int16_t> gridForward = _Vector3D<int16_t>(
        ray.dir.x >= 0.0f ? 1 : -1,
        ray.dir.y >= 0.0f ? 1 : -1,
        ray.dir.z >= 0.0f ? 1 : -1
        );

    Vector3D pos = Vector3D(grid.x * min_size.w, grid.y * min_size.h, grid.z * min_size.h) + rootAABB.bpos;
    Vector3D next_pos = pos;
    std::set<uint32_t> colliderList;

    while (true) {
        uint32_t number = SpaceOctree::Get3DMortonOrder(grid);

        int exists_max_split_level = 0;
        for (int i = 0; i <= factory->GetSplitLevel(); i++) {
            int split_level = factory->GetSplitLevel() - i;
            uint32_t idx = static_cast<uint32_t>((number >> i * 3) + PrecomputedConstants::PowNumbers<8, 8>::Get(split_level) / 7);
            if (factory->BoxExists(idx)) {
                colliderList.insert(idx);
                exists_max_split_level = std::max(exists_max_split_level, split_level);
            }
        }

        exists_max_split_level = std::min(exists_max_split_level + 1, factory->GetSplitLevel());
        auto next_grid = gridForward + factory->CalculateGridCoordinate(pos, exists_max_split_level);
        auto size = rootAABB.size() / static_cast<float>(1 << exists_max_split_level);
        next_pos = Vector3D(next_grid.x * size.w, next_grid.y * size.h, next_grid.z * size.h) + rootAABB.bpos;

        if (!rootAABB.Contains(next_pos)) {
            break;
        }

        float ax = ray.dir.x != 0.0f ? std::abs((next_pos.x - pos.x) / ray.dir.x) : FLT_MAX;
        float ay = ray.dir.y != 0.0f ? std::abs((next_pos.y - pos.y) / ray.dir.y) : FLT_MAX;
        float az = ray.dir.z != 0.0f ? std::abs((next_pos.z - pos.z) / ray.dir.z) : FLT_MAX;

        if (ax < ay && ax < az) {
            pos += ray.dir * ax;
            grid.x = next_grid.x;
        }
        else if (ay < ax && ay < az) {
            pos += ray.dir * ay;
            grid.y = next_grid.y;
        }
        else if (az < ax && az < ay) {
            pos += ray.dir * az;
            grid.z = next_grid.z;
        }
        else {
            pos += Vector3D(ray.dir.x * ax, ray.dir.y * ay, ray.dir.z * az);
            grid = next_grid;
        }
    }

    return colliderList;
}