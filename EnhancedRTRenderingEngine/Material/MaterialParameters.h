#pragma once

#include "Structure/Vector.h"

struct MaterialParameters {
    const char *vsName = nullptr, *psName = nullptr;
    const char *textureName = nullptr;
    Vector3D baseColor;
    float roughness, metallic;
};