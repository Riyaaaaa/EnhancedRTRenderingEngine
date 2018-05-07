#pragma once

#include <vector>

#include "Structure/Structure.h"

struct ExpandedTexel {
    int belongsTriangleIdx = -1;
    Vector4D worldPosition;
};

class ExpandMap {
public:
    ExpandMap(unsigned int size, unsigned int expand_triangle_nums) :
        _size(size),
        _map(size * size),
        _triangleNormals(expand_triangle_nums) {}

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

    void SetNormal(const Vector3D& normal, unsigned int triangle_idx) {
        _triangleNormals[triangle_idx] = normal;
    }

    const std::vector<_Vector2D<unsigned int>>& TriangleBaseIndices() const {
        return _triangleBaseIndices;
    }

    const Vector3D& GetNormal(unsigned int triangle_idx) const {
        return _triangleNormals[triangle_idx];
    }

protected:
    unsigned int _size;
    std::vector<Vector3D> _triangleNormals;
    std::vector<_Vector2D<unsigned int>> _triangleBaseIndices;
    std::vector<ExpandedTexel> _map;
};

struct StaticLightBuildData
{
public:
    std::vector<MainVertex> lightVertices;
};

