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

uint32_t SpaceOctree::Get3DMortonOrder(_Vector3D<uint8_t> coordinate) 
{
    return BitSeparateFor3D(coordinate.x) | BitSeparateFor3D(coordinate.y) << 1 | BitSeparateFor3D(coordinate.z) << 2;
}

OctreeFactoryBase::OctreeFactoryBase(AABB RootAABB, int splitLevel) :
_rootAABB(RootAABB),
_splitLevel(splitLevel),
_splitNums(static_cast<int>(std::pow(8, splitLevel + 1) / 7)) {
    
    _minBoxSize = RootAABB.size() / static_cast<float>((1 << splitLevel));
}

int OctreeFactoryBase::CalculateMortonNumber(const AABB& aabb) {
    uint32_t index1 = CalculateIndexFromPoint(aabb.bpos);
    uint32_t index2 = CalculateIndexFromPoint(aabb.epos);

    uint32_t Def = index1 ^ index2;
    unsigned int HiLevel = 1;
    int i;
    for (i = 0; i < _splitLevel; i++)
    {
        uint32_t Check = (Def >> (i * 3)) & 0x7;
        if (Check != 0)
            HiLevel = i + 1;
    }
    uint32_t SpaceNum = index2 >> (HiLevel * 3);
    uint32_t AddNum = static_cast<uint32_t>((PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(_splitLevel - HiLevel) - 1) / 7);
    SpaceNum += AddNum;

    if (SpaceNum > _splitNums)
        return 0xffffffff;

    return SpaceNum;
}

_Vector3D<int16_t> OctreeFactoryBase::CalculateGridCoordinate(const Vector3D& pos_) {
    Vector3D pos;
    pos.x = std::max(_rootAABB.bpos.x, std::min(pos_.x, _rootAABB.epos.x));
    pos.y = std::max(_rootAABB.bpos.y, std::min(pos_.y, _rootAABB.epos.y));
    pos.z = std::max(_rootAABB.bpos.z, std::min(pos_.z, _rootAABB.epos.z));

    return _Vector3D<int16_t>(
        static_cast<int16_t>((pos.x - _rootAABB.bpos.x) / _minBoxSize.w),
        static_cast<int16_t>((pos.y - _rootAABB.bpos.y) / _minBoxSize.h),
        static_cast<int16_t>((pos.z - _rootAABB.bpos.z) / _minBoxSize.d));
}

_Vector3D<int16_t> OctreeFactoryBase::CalculateGridCoordinate(const Vector3D& pos_, int split_level) {
    Vector3D pos;
    pos.x = std::max(_rootAABB.bpos.x, std::min(pos_.x, _rootAABB.epos.x));
    pos.y = std::max(_rootAABB.bpos.y, std::min(pos_.y, _rootAABB.epos.y));
    pos.z = std::max(_rootAABB.bpos.z, std::min(pos_.z, _rootAABB.epos.z));

    auto box_size = _rootAABB.size() / (1 << split_level);

    return _Vector3D<int16_t>(
        static_cast<int16_t>((pos.x - _rootAABB.bpos.x) / box_size.w),
        static_cast<int16_t>((pos.y - _rootAABB.bpos.y) / box_size.h),
        static_cast<int16_t>((pos.z - _rootAABB.bpos.z) / box_size.d));
}

int OctreeFactoryBase::CalculateMortonNumber(const Vector3D& pos, int splitLevel) {
    uint32_t index = CalculateIndexFromPoint(pos);
    uint32_t AddNum = static_cast<uint32_t>((PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(splitLevel) - 1) / 7);

    return index + AddNum;
}

int OctreeFactoryBase::CalculateIndexFromPoint(const Vector3D& pos) {
    return Get3DMortonOrder(CalculateGridCoordinate(pos));
}

AABB OctreeFactoryBase::CalculateOctreeBoxAABBFromMortonNumber(uint32_t number) const {
    int level = 0;
    while (number >= PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(level)) {
        number -= static_cast<uint32_t>(PrecomputedConstants::PowNumbers<8, MaxSpaceSeparateNums>::Get(level));
        level++;
    }

    uint32_t s = 0;

    for (int i = level; i > 0; i--) {
        auto tmp = (number >> (3 * i - 2 - i) & (1 << (i - 1)));
        s = s | tmp;
    }
    uint32_t x = s;

    s = 0;
    for (int i = level; i > 0; i--) {
        s = s | (number >> (3 * i - 1 - i) & (1 << (i - 1)));
    }
    uint32_t y = s;

    s = 0;
    for (int i = level; i > 0; i--) {
        s = s | (number >> (3 * i - i) & (1 << (i - 1)));
    }
    uint32_t z = s;

    Size3D boxSize = _rootAABB.size() / static_cast<float>((1 << level));
    Vector3D bpos = Vector3D(x * boxSize.w, y * boxSize.h, z * boxSize.d) + _rootAABB.bpos;
    
    return AABB(bpos, Vector3D(bpos.x + boxSize.w, bpos.y + boxSize.h, bpos.z + boxSize.d));
}
