#pragma once

#include "Structure/Structure.h"

namespace MathUtils
{
    float Dot(const Vector3D& v1, const Vector3D& v2);
    Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
    Vector3D Normalize(const Vector3D& v);
    Vector3D Reflect(const Vector3D& v, const Vector3D& nor);

    float Determinant3x3(const Vector3D& a1, const Vector3D& a2, const Vector3D& a3);
}

