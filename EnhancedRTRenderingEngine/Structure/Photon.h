#pragma once

#include "Structure.h"

struct Photon {
    Vector3D pos;
    Vector3D power;
    Vector3D incident;

    Photon(const Vector3D& position_, const Vector3D& power_, const Vector3D& incident_) :
        pos(position_), power(power_), incident(incident_) {}
};
