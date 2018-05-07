#pragma once

#include "Vector.h"
#include "Structure.h"

struct Segment {
    Segment() = default;
    Segment(const Vector3D& b, const Vector3D& e) : bpos(b), epos(e) {}
    Vector3D bpos, epos;
};

struct Line {
    Line() = default;
    Line(Segment _seg, Color3B _color, float _thickness) : seg(_seg), color(_color), thickness(_thickness) {}
    Segment seg;
    Color3B color;
    float thickness;
};

struct Triangle {
    Vector3D v0, v1, v2;
    Vector3D normal;
};
