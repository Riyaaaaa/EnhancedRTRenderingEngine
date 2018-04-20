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

    void AddTriangle(_Vector2D<unsigned int> v0, _Vector2D<unsigned int> v1, _Vector2D<unsigned int> v2) {
        _triangleBaseIndices.push_back(v0);
        _triangleBaseIndices.push_back(v1);
        _triangleBaseIndices.push_back(v2);
    }

    const std::vector<_Vector2D<unsigned int>>& TriangleBaseIndices() const {
        return _triangleBaseIndices;
    }

protected:
    unsigned int _size;
    std::vector<_Vector2D<unsigned int>> _triangleBaseIndices;
    std::vector<ExpandedTexel> _map;
};

struct StaticLightBuildData
{
public:
    std::vector<Vector2D> lightMapUVs;
};

