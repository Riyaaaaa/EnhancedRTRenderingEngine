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

static void RasterizeTriangle01(ExpandMap& map, const std::array<_Vector2D<long>, 3>& rasterPositions, Triangle& tri, int idx) {
    float a = (rasterPositions[2].y - rasterPositions[1].y) / (float)(rasterPositions[2].x - rasterPositions[1].x);
    float b = rasterPositions[1].y - a * rasterPositions[1].x;

    _Vector2D<long> ac = rasterPositions[2] - rasterPositions[0];
    _Vector2D<long> ab = rasterPositions[1] - rasterPositions[0];

    Vector4D v2v0 = tri.v2 - tri.v0;
    Vector4D v1v0 = tri.v1 - tri.v0;

    float dot00 = Dot(ac, ac);
    float dot01 = Dot(ac, ab);
    float dot11 = Dot(ab, ab);
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

    for (long y = rasterPositions[0].y; y <= rasterPositions[2].y; y++) {
        for (long x = rasterPositions[0].x; x <= (y - b) / a; x++) {
            
            _Vector2D<long> ap = _Vector2D<long>(x, y) - rasterPositions[0];
            float dot02 = Dot(ac, ap);
            float dot12 = Dot(ab, ap);

            float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

            map(x, y).worldPosition = v2v0 * u + v1v0 * v;
            map(x, y).belongsTriangleIdx = idx;
        }
    }
}

static void RasterizeTriangle02(ExpandMap& map, const std::array<_Vector2D<long>, 3>& rasterPositions, Triangle& tri, int idx) {
    float a = (rasterPositions[1].y - rasterPositions[0].y) / (float)(rasterPositions[1].x - rasterPositions[0].x);
    float b = rasterPositions[0].y - a * rasterPositions[0].x;

    _Vector2D<long> ac = rasterPositions[2] - rasterPositions[0];
    _Vector2D<long> ab = rasterPositions[1] - rasterPositions[0];

    Vector4D v2v0 = tri.v2 - tri.v0;
    Vector4D v1v0 = tri.v1 - tri.v0;

    float dot00 = Dot(ac, ac);
    float dot01 = Dot(ac, ab);
    float dot11 = Dot(ab, ab);
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);

    for (long y = rasterPositions[0].y; y >= rasterPositions[1].y; y--) {
        for (long x = (y - b) / a; x <= rasterPositions[1].x; x++) {
            _Vector2D<long> ap = _Vector2D<long>(x, y) - rasterPositions[0];
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

    auto bake_triangle_size = _expandSize / (int)(std::sqrtf((triangles.size() / 2)) + 1) - _margin;

    unsigned int offsetX = 0;
    unsigned int offsetY = _margin;

    float _expandSizef = (float)_expandSize;

    for (unsigned int i = 0; i < triangles.size(); i++) {
        auto& tri = triangles[i];
        
        std::array<_Vector2D<long>, 3> mappedVertPositions;
        
        if (i % 2 == 0) {
            offsetX += _margin;
            mappedVertPositions = std::array<_Vector2D<long>, 3> {
                _Vector2D<long>(offsetX, offsetY),
                    _Vector2D<long>((offsetX + bake_triangle_size - 1), offsetY),
                    _Vector2D<long>(offsetX, (offsetY + bake_triangle_size - 1))
            };
            RasterizeTriangle01(map, mappedVertPositions, tri, i);
        }
        else {
            mappedVertPositions = std::array<_Vector2D<long>, 3> {
                _Vector2D<long>(offsetX, (offsetY + bake_triangle_size - 1)),
                    _Vector2D<long>((offsetX + bake_triangle_size - 1), offsetY),
                    _Vector2D<long>((offsetX + bake_triangle_size - 1), (offsetY + bake_triangle_size - 1))
            };
            RasterizeTriangle02(map, mappedVertPositions, tri, i);
            offsetX += bake_triangle_size;
            if (offsetX >= _expandSize - bake_triangle_size - _margin) {
                offsetX = 0;
                offsetY += bake_triangle_size + _margin;
            }
        }
 
        map.AddTriangle(mappedVertPositions[0], mappedVertPositions[1], mappedVertPositions[2]);
    }

    return map;
}
