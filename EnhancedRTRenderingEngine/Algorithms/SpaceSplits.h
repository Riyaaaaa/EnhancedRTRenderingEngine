#pragma once

#include "Structure/Structure.h"

namespace SpaceSplits {
    template<class VertType>
    AABB CaluclateOptimizedAABB(const std::vector<VertType>& vertices) {
        Vector3D min(0,0,0), max(FLT_MAX, FLT_MAX, FLT_MAX);
        for (auto && vert : vertices) {
            if (min.x > vert.pos.x) {
                min.x = vert.pos.x;
            }

            if (min.y > vert.pos.y) {
                min.y = vert.pos.y;
            }

            if (min.z > vert.pos.z) {
                min.z = vert.pos.z;
            }

            if (max.x < vert.pos.x) {
                max.x = vert.pos.x;
            }

            if (max.y < vert.pos.y) {
                max.y = vert.pos.y;
            }

            if (max.z < vert.pos.z) {
                max.z = vert.pos.z;
            }
        }
    }
}

