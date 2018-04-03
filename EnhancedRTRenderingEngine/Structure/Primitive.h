#pragma once

#include "Vector.h"
#include "Structure.h"

struct Line {
    Vector3D bpos, epos;
    Color3B color;
    float thickness;
};
