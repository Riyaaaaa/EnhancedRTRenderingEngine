#pragma once

#include <DirectXMath.h>

struct Size {
	float w, h;
};

struct Vector3D {
	float x, y, z;
};

struct Vector4D {
	Vector4D() {}
	Vector4D(const Vector3D vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f) {}
	float x, y, z, w;
};

struct Vector2D {
	float x, y;

	Vector2D()
		: x(0.0f), y(0.0f)
	{}

	Vector2D(float xx, float yy)
		: x(xx), y(yy)
	{}

	  float distance(const Vector2D& v) const {
		return sqrtf(powf(v.x - this->x, 2) + powf(v.y - this->y, 2));
	}

	  Vector2D operator+(const Vector2D& v) const {
		return Vector2D(x + v.x, y + v.y);
	}

	  Vector2D operator-(const Vector2D &v) const {
		return Vector2D(this->x - v.x, this->y - v.y);
	}

	  Vector2D operator-() const {
		return Vector2D(-this->x, -this->y);
	}

	  Vector2D operator*(float s) const {
		return Vector2D(this->x * s, this->y * s);
	}

	  Vector2D operator/(float s) const {
		return Vector2D(this->x / s, this->y / s);
	}

	  bool operator<(const Vector2D& v) const {
		return (x == v.x) ? y < v.y : x < v.x;
	}

	  bool operator>(const Vector2D& v) const {
		return (x == v.x) ? y > v.y : x > v.x;
	}

	  bool operator==(const Vector2D& v) const {
		return x == v.x && y == v.y;
	}

	  bool operator!=(const Vector2D& v) const {
		return x != v.x || y != v.y;
	}

	  Vector2D& operator-=(const Vector2D& v) {
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	  Vector2D& operator+=(const Vector2D& v) {
		this->x = this->x + v.x;
		this->y = this->y + v.y;

		return *this;
	}

	  Vector2D& operator*=(float s) {
		this->x *= s;
		this->y *= s;
		return *this;
	}

	  Vector2D& operator/=(float s) {
		this->x /= s;
		this->y /= s;
		return *this;
	}
};

struct Color4B {
	float r, g, b, a;
};

struct SimpleVertex {
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
};

struct Vertex3D {
	float pos[3];	//x-y-z
	float col[4];	//r-g-b-a
	float tex[2];	//x-y
};

struct PMDVertex {
	float pos[3];	//x-y-z
	float tex[2];	//x-y
	float normal[3];
};

struct Transform {
	Vector3D location;
	Vector3D rotation;
	Vector3D scale;
};

struct PointLightParameters {
	Vector4D pos;
	// X: constant attenuation factor, Y:  linear attenuation factor, Z: quadratic attenuation factor
	Vector4D attenuation;
};

struct TransformBufferParam
{
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
};

struct ConstantBuffer
{
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;
	Vector4D DirectionalLight;
	PointLightParameters PointLight;
};

