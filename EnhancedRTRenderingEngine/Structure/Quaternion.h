#pragma once

#include "Vector.h"

struct Quaternion
{
public:
    static Quaternion FromEuler(float x, float y, float z);
    Quaternion(float x, float y, float z, float w);
    Quaternion(Vector3D axis, float angle);
    Quaternion();

    float AsConjugate() const;
    float Length() const;
    float x, y, z, w;
};

