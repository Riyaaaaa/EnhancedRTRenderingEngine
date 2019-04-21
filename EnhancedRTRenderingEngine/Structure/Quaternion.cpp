#include "Quaternion.h"

#include "Constant/MathConstant.h"

Quaternion Quaternion::FromEuler(float x, float y, float z)
{
	x *= Constant::DEG_TO_RADIAN;
	y *= Constant::DEG_TO_RADIAN;
	z *= Constant::DEG_TO_RADIAN;

	float c1 = std::cos(x * 0.5f);
	float c2 = std::cos(y * 0.5f);
	float c3 = std::cos(z * 0.5f);

	float s1 = std::sin(x * 0.5f);
	float s2 = std::sin(y * 0.5f);
	float s3 = std::sin(z * 0.5f);

	// YXZ
	float qx = s1 * c2 * c3 + c1 * s2 * s3;
	float qy = c1 * s2 * c3 - s1 * c2 * s3;
	float qz = c1 * c2 * s3 - s1 * s2 * c3;
	float qw = c1 * c2 * c3 + s1 * s2 * s3;

	return Quaternion(qx, qy, qz, qw);
}

Quaternion::Quaternion()
{
	*this = FromEuler(0, 0, 0);
}

Quaternion::Quaternion(float x, float y, float z, float w)
	: x(x)
	, y(y)
	, z(z)
	, w(w)
{
}

Quaternion::Quaternion(Vector3D axis, float angle)
{
	x = axis.x * sin(angle / 2.0f);
	y = axis.y * sin(angle / 2.0f);
	z = axis.z * sin(angle / 2.0f);
	w = cos(angle / 2.0f);
}

float Quaternion::AsConjugate() const
{
	return x * x + y * y + z * z + w * w;
}

float Quaternion::Length() const
{
	return std::sqrt(AsConjugate());
}