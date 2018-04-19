#include "stdafx.h"
#include "MathUtils.h"

#include <cmath>

float MathUtils::Dot(const Vector2D& v1, const Vector2D& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

float MathUtils::Dot(const Vector3D& v1, const Vector3D& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D MathUtils::Cross(const Vector3D& v1, const Vector3D& v2) {
    return Vector3D{v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x};
}

Vector3D MathUtils::Normalize(const Vector3D& v) {
    float length = std::sqrt(std::pow(v.x, 2) + std::pow(v.y, 2) + std::pow(v.z, 2));
    return Vector3D{v.x / length, v.y / length, v.z / length};
}

float MathUtils::Determinant3x3(const Vector3D& a1, const Vector3D& a2, const Vector3D& a3) {
    return ((a1.x * a2.y * a3.z) + (a1.y * a2.z * a3.x) + (a1.z * a2.x * a3.y)
        - (a1.x * a2.z * a3.y) - (a1.y * a2.x * a3.z) - (a1.z * a2.y * a3.x));
}

Vector3D MathUtils::Reflect(const Vector3D& v, const Vector3D& nor) {
    return v + nor * 2 * Dot(-v, nor);
}

float MathUtils::DistanceSq(const Vector3D& v1, const Vector3D& v2) {
    return std::pow((v2.x - v1.x), 2) + std::pow((v2.y - v1.y), 2) + std::pow((v2.z - v1.z), 2);
}