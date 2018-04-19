#pragma once

#include <vector>

#include "Structure/Structure.h"

struct ExpandedTexel {
    int belongsTriangleIdx = -1;
    Vector4D worldPosition;
};

class ExpandMap {
public:
    ExpandMap(unsigned int size) :
        _size(size),
        _map(size * size) {}

    ExpandedTexel& operator()(unsigned int x, unsigned int y) {
        return _map[y * _size + x];
    }

    ExpandedTexel& operator()(unsigned int idx) {
        return _map[idx];
    }

protected:
    unsigned int _size;
    std::vector<ExpandedTexel> _map;
};

class StaticLightBuildData
{
public:
    ExpandMap _mappedMeshMap;
    std::vector<LightVertex> _lightVerts;
};

