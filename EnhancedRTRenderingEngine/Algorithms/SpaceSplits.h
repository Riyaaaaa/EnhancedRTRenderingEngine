#pragma once

#include <vector>

#include "Structure/Structure.h"

namespace SpaceSplits {
    template<class VertType>
    AABB CalculateOptimizedAABB(const std::vector<VertType>& vertices) {
        Vector3D Min(FLT_MAX, FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        for (auto && vert : vertices) {
            if (Min.x > vert.pos.x) {
                Min.x = vert.pos.x;
            }

            if (Min.y > vert.pos.y) {
                Min.y = vert.pos.y;
            }

            if (Min.z > vert.pos.z) {
                Min.z = vert.pos.z;
            }

            if (Max.x < vert.pos.x) {
                Max.x = vert.pos.x;
            }

            if (Max.y < vert.pos.y) {
                Max.y = vert.pos.y;
            }

            if (Max.z < vert.pos.z) {
                Max.z = vert.pos.z;
            }
        }

        return AABB(Min, Max);
    }

    AABB CalculateOptimizedAABB(const std::vector<Vector3D>& vertices);

    AABB CalculateOptimizedAABB(const std::vector<AABB>& aabbs);
}

