#include "Structure.h"

#include <algorithm>

#include "Utility/MathUtils.h"

void Transform::LookAt(const Vector3D& other) {
    Matrix4 m;
    m.LookAt(location, other, Vector::UP);

    float elem[4];
    elem[0] = m(0, 0) - m(1, 1) - m(2, 2) + 1.0f;
    elem[1] = -m(0, 0) + m(1, 1) - m(2, 2) + 1.0f;
    elem[2] = -m(0, 0) - m(1, 1) + m(2, 2) + 1.0f;
    elem[3] = m(0, 0) + m(1, 1) + m(2, 2) + 1.0f;

    std::size_t biggestIdx = std::distance(&elem[0], std::max_element(&elem[0], &elem[3]));

    float v = std::sqrtf(elem[biggestIdx]) * 0.5f;
    float mult = 0.25 / v;

    switch (biggestIdx)
    {
    case 0:
        rotation.y = (m(1, 0) + m(0, 1)) * mult;
        rotation.z = (m(0, 2) + m(2, 0)) * mult;
        rotation.w = (m(2, 1) - m(1, 2)) * mult;
        break;
    case 1:
        rotation.x = (m(1, 0) + m(0, 1)) * mult;
        rotation.z = (m(2, 1) + m(1, 2)) * mult;
        rotation.w = (m(0, 2) - m(2, 0)) * mult;
        break;
    case 2:
        rotation.x = (m(0, 2) + m(2, 0)) * mult;
        rotation.y = (m(2, 1) + m(1, 2)) * mult;
        rotation.w = (m(1, 0) - m(0, 1)) * mult;
        break;
    case 3:
        rotation.x = (m(2, 1) + m(1, 2)) * mult;
        rotation.y = (m(0, 2) + m(2, 0)) * mult;
        rotation.z = (m(1, 0) + m(0, 1)) * mult;
        break;
    }
}
