#pragma once

#include "Structure/Structure.h"

class DirectionalLight
{
public:
    DirectionalLight(const Vector3D& dir);
    ~DirectionalLight();

    void SetDirection(const Vector3D& dir) { _direction = dir; }
    const Vector3D& GetDirection() { return _direction; }

    Vector3D _direction;
};

