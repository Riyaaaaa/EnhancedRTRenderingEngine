#include "stdafx.h"

#include <array>

#include "MeshExpander.h"

#include "StaticLightBuildData.h"

#include "Utility/MathUtils.h"

using namespace MathUtils;

MeshExpander::MeshExpander(unsigned int size, unsigned int margin) :
    _expandSize(size),
    _margin(margin) {

}

static void RasterizeTriangle(ExpandMap& map, const std::array<_Vector2D<unsigned int>, 3>& rasterPositions, Triangle& tri, int idx) {
    float a = (rasterPositions[2].y - rasterPositions[1].y) / (rasterPositions[2].x - rasterPositions[1].x);
    float b = rasterPositions[0].y - a * rasterPositions[0].x;

    _Vector2D<unsigned int> ac = rasterPositions[2] - rasterPositions[0];
    _Vector2D<unsigned int> ab = rasterPositions[1] - rasterPositions[0];

    Vector4D v2v0 = tri.v2 - tri.v0;
    Vector4D v1v0 = tri.v1 - tri.v0;

    float dot00 = Dot(ac, ac);
    float dot01 = Dot(ac, ab);
    float dot11 = Dot(ab, ab);
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

    for (unsigned int y = rasterPositions[0].y; y < rasterPositions[2].y; y++) {
        for (unsigned int x = rasterPositions[0].x; x < (y - b) / a; x++) {
            
            _Vector2D<unsigned int> ap = _Vector2D<unsigned int>(x, y) - rasterPositions[0];
            float dot02 = Dot(ac, ap);
            float dot12 = Dot(ab, ap);

            float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

            map(x, y).worldPosition = v2v0 * u + v1v0 * v;
            map(x, y).belongsTriangleIdx = idx;
        }
    }
}

ExpandMap MeshExpander::Build(MeshBase* mesh) {
    ExpandMap map(_expandSize);

    auto triangles = mesh->GetTriangles();

    auto bake_triangle_size = _expandSize * _expandSize / (triangles.size() / 2) - _margin;

    unsigned int offsetX = 0;
    unsigned int offsetY = 0;

    float _expandSizef = (float)_expandSize;

    for (unsigned int i = 0; i < triangles.size(); i++) {
        auto& tri = triangles[i];
        offsetX += _margin;
        offsetY += _margin;

        std::array<_Vector2D<unsigned int>, 3> mappedVertPositions = {
            _Vector2D<unsigned int>(offsetX / _expandSizef, offsetY / _expandSizef) ,
            _Vector2D<unsigned int>((offsetX + bake_triangle_size) / _expandSizef, offsetY / _expandSizef),
            _Vector2D<unsigned int>(offsetX / _expandSizef, (offsetY + bake_triangle_size) / _expandSizef)
        };

        RasterizeTriangle(map, mappedVertPositions, tri, i);
        map.AddTriangle(mappedVertPositions[0], mappedVertPositions[1], mappedVertPositions[2]);
    }

    return map;
}
