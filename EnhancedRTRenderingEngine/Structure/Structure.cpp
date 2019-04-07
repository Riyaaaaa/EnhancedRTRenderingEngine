#include "Structure.h"

#include "Utility/MathUtils.h"

void Transform::LookAt(const Vector3D& other) {
	Vector3D z = Math::Normalize(other - location);
	Vector3D x = Math::Normalize(Math::Cross(Vector::UP, z));
	Vector3D y = Math::Normalize(Math::Cross(z, x));

	Matrix m;
	m.at(0, 0) = x.x; m.at(0, 1) = y.x; m.at(0, 2) = z.x;
	m.at(1, 0) = x.y; m.at(1, 1) = y.y; m.at(1, 2) = z.y;
	m.at(2, 0) = x.z; m.at(2, 1) = y.z; m.at(2, 2) = z.z;
	// TODO:
}