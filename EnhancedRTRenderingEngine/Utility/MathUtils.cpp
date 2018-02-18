#include "stdafx.h"
#include "MathUtils.h"

#include <cmath>


Vector3D MathUtils::Cross(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D{v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x};
}

Vector3D MathUtils::Normalize(const Vector3D& v) {
    float length = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2));
    return Vector3D{v.x / length, v.y / length, v.z / length};
}