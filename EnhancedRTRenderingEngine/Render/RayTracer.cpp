#include "stdafx.h"

#include "RayTracer.h"

#include "Constant/PrecomputedConstants.h"

std::set<uint32_t> GetColliderMeshList(SpaceOctree::OctreeFactoryBase* factory, Vector3D bpos, Vector3D dir) {
    auto size = factory->GetMinBoxSize();
    auto rayForward = Vector3D(dir.x * size.w, dir.y * size.h, dir.z * size.d);
    auto rootAABB = factory->GetRootAABB();
    
    _Vector3D<int16_t> grid = factory->CalculateGridCoordinate(bpos);
    _Vector3D<int8_t> gridForward = _Vector3D<int8_t>(
        dir.x >= 0.0f ? 1 : -1,
        dir.y >= 0.0f ? 1 : -1,
        dir.z >= 0.0f ? 1 : -1
        );

    Vector3D pos = Vector3D(grid.x * size.w, grid.y * size.h, grid.z * size.h) + rootAABB.bpos;
    _Vector3D<int16_t> nextGrid = grid;
    std::set<uint32_t> colliderList;

    while (rootAABB.Contains(pos)) {
        uint32_t number = SpaceOctree::Get3DMortonOrder(grid);

        for (int i = 0; i <= factory->GetSplitLevel(); i++) {
            uint32_t idx = (number >> i * 3) + PrecomputedConstants::PowNumbers<8, 8>::Get(factory->GetSplitLevel() - i) / 7;
            if (factory->BoxExists(idx)) {
                colliderList.insert(idx);
            }
        }

        nextGrid = grid + gridForward;
        Vector3D nextpos = Vector3D(nextGrid.x * size.w, nextGrid.y * size.h, nextGrid.z * size.h) + rootAABB.bpos;

        float ax = dir.x != 0.0f ? std::abs((nextpos.x - pos.x) / rayForward.x) : FLT_MAX;
        float ay = dir.y != 0.0f ? std::abs((nextpos.y - pos.y) / rayForward.y) : FLT_MAX;
        float az = dir.z != 0.0f ? std::abs((nextpos.z - pos.z) / rayForward.z) : FLT_MAX;

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