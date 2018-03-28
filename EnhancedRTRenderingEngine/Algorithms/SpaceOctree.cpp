#include "stdafx.h"
#include "SpaceOctree.h"

#include "Constant/PrecomputedConstants.h"

using namespace SpaceOctree;

uint32_t SpaceOctree::BitSeparateFor3D(uint8_t n)
{
    uint32_t s = n;
    s = (s | s << 8) & 0x0000f00f;
    s = (s | s << 4) & 0x000c30c3;
    s = (s | s << 2) & 0x00249249;
    return s;
}

uint32_t SpaceOctree::Get3DMortonOrder(uint8_t x, uint8_t y, uint8_t z)
{
    return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
}

LinerOctreeFactory::LinerOctreeFactory(AABB RootAABB, int splitLevel) :
_rootAABB(RootAABB),
_splitLevel(splitLevel),
_splitNums(std::pow(8, splitLevel + 1) / 7),
_linerTree(_splitNums) {
    
    _minBoxSize = RootAABB.size() / (1 << splitLevel);
}

int LinerOctreeFactory::CalculateMortonNumber(const AABB& aabb) {
    uint32_t index1 = CalculateIndexFromPoint(aabb.bpos);
    uint32_t index2 = CalculateIndexFromPoint(aabb.epos);

    uint32_t Def = index1 ^ index2;
    unsigned int HiLevel = 1;
    unsigned int i;
    for (i = 0; i < _splitLevel; i++)
    {
        uint32_t Check = (Def >> (i * 3)) & 0x7;
        if (Check != 0)
            HiLevel = i + 1;
    }
    uint32_t SpaceNum = index2 >> (HiLevel * 3);
    uint32_t AddNum = (PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(_splitLevel - HiLevel) - 1) / 7;
    SpaceNum += AddNum;

    if (SpaceNum > _splitNums)
        return 0xffffffff;

    return SpaceNum;
}

int LinerOctreeFactory::CalculateIndexFromPoint(const Vector3D& pos) {
    return Get3DMortonOrder(
        (pos.x - _rootAABB.bpos.x) / _minBoxSize.w,
        (pos.y - _rootAABB.bpos.y) / _minBoxSize.h,
        (pos.z - _rootAABB.bpos.z) / _minBoxSize.d
    );
}

AABB LinerOctreeFactory::CalculateOctreeBoxAABBFromMortonNumber(uint32_t number) const {
    int level = 0;
    while (number >= PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(level)) {
        number -= PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(level);
        level++;
    }

    uint32_t s = 0;

    for (int i = level; i > 0; i--) {
        auto tmp = (number >> (3 * i - 2 - i) & (1 << i - 1));
        s = s | tmp;
    }
    uint32_t x = s;

    s = 0;
    for (int i = level; i > 0; i--) {
        s = s | (number >> (3 * i - 1 - i) & (1 << i - 1));
    }
    uint32_t y = s;

    s = 0;
    for (int i = level; i > 0; i--) {
        s = s | (number >> (3 * i - i) & (1 << i - 1));
    }
    uint32_t z = s;

    Size3D boxSize = _rootAABB.size() / (1 << level);;
    Vector3D bpos = Vector3D(x * boxSize.w, y * boxSize.h, z * boxSize.d) + _rootAABB.bpos;
    
    return AABB(bpos, Vector3D(bpos.x + boxSize.w, bpos.y + boxSize.h, bpos.z + boxSize.d));
}
