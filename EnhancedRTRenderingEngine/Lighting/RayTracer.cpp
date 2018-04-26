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

std::vector<Segment> RayTraceIf(SpaceOctree::OctreeFactoryBase* factory, Ray ray, std::function<bool(const Material&, int)> hit_program) {
    std::vector<Segment> rayRoutes;

    Material mat;
    int trace_count = 0;
    do {
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
    } while (hit_program(mat, trace_count));

    return rayRoutes;
}

std::set<uint32_t> GetColliderMortonList(SpaceOctree::OctreeFactoryBase* factory, Ray ray) {
    auto size = factory->GetMinBoxSize();
    auto rayForward = Vector3D(ray.dir.x * size.w, ray.dir.y * size.h, ray.dir.z * size.d);
    auto rootAABB = factory->GetRootAABB();
    
    _Vector3D<int16_t> grid = factory->CalculateGridCoordinate(ray.pos);
    _Vector3D<int8_t> gridForward = _Vector3D<int8_t>(
        ray.dir.x >= 0.0f ? 1 : -1,
        ray.dir.y >= 0.0f ? 1 : -1,
        ray.dir.z >= 0.0f ? 1 : -1
        );

    Vector3D pos = Vector3D(grid.x * size.w, grid.y * size.h, grid.z * size.h) + rootAABB.bpos;
    _Vector3D<int16_t> nextGrid = grid;
    std::set<uint32_t> colliderList;

    while (rootAABB.Contains(pos)) {
        uint32_t number = SpaceOctree::Get3DMortonOrder(grid);

        for (int i = 0; i <= factory->GetSplitLevel(); i++) {
            uint32_t idx = static_cast<uint32_t>((number >> i * 3) + PrecomputedConstants::PowNumbers<8, 8>::Get(factory->GetSplitLevel() - i) / 7);
            if (factory->BoxExists(idx)) {
                colliderList.insert(idx);
            }
        }

        nextGrid = grid + gridForward;
        Vector3D nextpos = Vector3D(nextGrid.x * size.w, nextGrid.y * size.h, nextGrid.z * size.h) + rootAABB.bpos;

        float ax = ray.dir.x != 0.0f ? std::abs((nextpos.x - pos.x) / rayForward.x) : FLT_MAX;
        float ay = ray.dir.y != 0.0f ? std::abs((nextpos.y - pos.y) / rayForward.y) : FLT_MAX;
        float az = ray.dir.z != 0.0f ? std::abs((nextpos.z - pos.z) / rayForward.z) : FLT_MAX;

        if (ax < ay && ax < az) {
            pos += rayForward * ax;
            grid.x += gridForward.x;
        }
        else if (ay < ax && ay < az) {
            pos += rayForward * ay;
            grid.y += gridForward.y;
        }
        else if (az < ax && az < ay) {
            pos += rayForward * az;
            grid.z += gridForward.z;
        }
        else {
            pos += rayForward;
            grid += gridForward;
        }
    }

    return colliderList;
}