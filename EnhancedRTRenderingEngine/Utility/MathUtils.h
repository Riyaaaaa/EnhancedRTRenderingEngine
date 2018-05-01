#pragma once

#include "Structure/Structure.h"

namespace MathUtils
{
    float Dot(const Vector2D& v1, const Vector2D& v2);
    float Dot(const Vector3D& v1, const Vector3D& v2);

    template<class T, std::size_t Dim>
    T Dot(const _Vector<T, Dim>& v1, const _Vector<T, Dim>& v2) {
        T dot = 0;
        for (std::size_t i = 0; i < Dim; i++) {
            dot += v1[i] * v2[i];
        }
        return dot;
    }

    Vector3D Multiply(const Vector3D& v1, const Vector3D& v2);
    Vector3D Cross(const Vector3D& v1, const Vector3D& v2);
    Vector3D Normalize(const Vector3D& v);
    Vector3D Reflect(const Vector3D& v, const Vector3D& nor);
    float DistanceSq(const Vector3D& v1, const Vector3D& v2);

    float Determinant3x3(const Vector3D& a1, const Vector3D& a2, const Vector3D& a3);
}

