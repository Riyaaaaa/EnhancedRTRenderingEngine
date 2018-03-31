#include "stdafx.h"
#include "SpaceSplits.h"

AABB SpaceSplits::CalculateOptimizedAABB(const std::vector<Vector3D>& vertices) {
    Vector3D Min(FLT_MAX, FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (auto && vert : vertices) {
        if (Min.x > vert.x) {
            Min.x = vert.x;
        }

        if (Min.y > vert.y) {
            Min.y = vert.y;
        }

        if (Min.z > vert.z) {
            Min.z = vert.z;
        }

        if (Max.x < vert.x) {
            Max.x = vert.x;
        }

        if (Max.y < vert.y) {
            Max.y = vert.y;
        }

        if (Max.z < vert.z) {
            Max.z = vert.z;
        }
    }

    return AABB(Min, Max);
}

AABB SpaceSplits::CalculateOptimizedAABB(const std::vector<AABB>& aabbs) {
    Vector3D Min(FLT_MAX, FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    for (auto && aabb : aabbs) {
        if (Min.x > aabb.bpos.x) {
            Min.x = aabb.bpos.x;
        }

        if (Min.y > aabb.bpos.y) {
            Min.y = aabb.bpos.y;
        }

        if (Min.z > aabb.bpos.z) {
            Min.z = aabb.bpos.z;
        }

        if (Max.x < aabb.epos.x) {
            Max.x = aabb.epos.x;
        }

        if (Max.y < aabb.epos.y) {
            Max.y = aabb.epos.y;
        }

        if (Max.z < aabb.epos.z) {
            Max.z = aabb.epos.z;
        }
    }

    return AABB(Min, Max);
}