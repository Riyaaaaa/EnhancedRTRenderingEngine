#pragma once

#include <cstdint>

#include "Scene/MeshObject.h"
#include "SpiralLibrary/Common/Math.hpp"
#include "Structure/Octree.h"

namespace SpaceOctree {
    constexpr int MaxSpaceSeparateNums = 8;

    uint32_t BitSeparateFor3D(uint8_t n);
    uint32_t Get3DMortonOrder(uint8_t x, uint8_t y, uint8_t z);

    class LinerOctreeFactory {
    public:
        LinerOctreeFactory(AABB RootAABB, int splitLevel);

        bool Register(IMeshObject* object)
        {
            auto aabb = object->GetAABB();
            uint32_t Elem = CalculateMortonNumber(aabb);
            if (Elem < _splitNums) {
                if (!_linerTree[Elem]) {
                    SetBox(Elem);
                }
                _linerTree[Elem]->Push(std::make_shared<OctreeNode>(object));
                return true;
            }
            return false;
        }

        void SetBox(uint32_t Elem) {
            while (!_linerTree[Elem])
            {
                _linerTree[Elem] = std::make_unique<OctreeBox>();
                Elem = (Elem - 1) >> 3;
                if (Elem >= _splitNums) break;
            }
        }

        AABB CalculateOctreeBoxAABBFromMortonNumber(uint32_t number) const;
        int CalculateMortonNumber(const AABB& aabb);
        int CalculateIndexFromPoint(const Vector3D& pos);

        const std::vector<std::unique_ptr<OctreeBox>>& GetTree() const {
            return _linerTree;
        }
    private:
        uint32_t _splitNums;
        int _splitLevel;
        Size3D _minBoxSize;
        std::vector<std::unique_ptr<OctreeBox>> _linerTree;
        AABB _rootAABB;
    };
}