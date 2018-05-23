#pragma once

#include "Vector.h"

struct Ray {
    Ray() = default;
    Ray(const Vector3D& p, const Vector3D& d) : pos(p), dir(d) {}
    Vector3D pos;
    Vector3D dir;
};

struct Hit {
    Vector3D pos;
    Vector3D incident;
    Vector3D nextDir;
    float distance;
    unsigned int materialIdx;
    Hit() = default;
    Hit(const Vector3D& p, const Vector3D& dir, const Vector3D& incident, float d, unsigned int matIdx) : pos(p), nextDir(dir), incident(incident), distance(d), materialIdx(matIdx) {}
};

