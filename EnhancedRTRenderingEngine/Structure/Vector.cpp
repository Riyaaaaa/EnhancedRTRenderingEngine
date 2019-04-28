
#include "Vector.h"

#include "Utility/MathUtils.h"

template<>
void Matrix4::LookAt(const Vector3D& eye, const Vector3D& target, const Vector3D& up) {
    Vector3D z = Math::Normalize(target - eye);
    Vector3D x = Math::Normalize(Math::Cross(up, z));
    Vector3D y = Math::Normalize(Math::Cross(z, x));

    at(0, 0) = x.x; at(0, 1) = y.x; at(0, 2) = z.x;
    at(1, 0) = x.y; at(1, 1) = y.y; at(1, 2) = z.y;
    at(2, 0) = x.z; at(2, 1) = y.z; at(2, 2) = z.z;
}