#pragma once

#include <cmath>

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

struct Vector3D {
    float x, y, z;

    Vector3D() {}
    constexpr Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

    Vector3D operator-(const Vector3D &v) const {
        return Vector3D{ this->x - v.x, this->y - v.y, this->z - v.z };
    }

    Vector3D operator+(const Vector3D &v) const {
        return Vector3D{ this->x + v.x, this->y + v.y, this->z + v.z };
    }

    Vector3D operator-() const {
        return Vector3D{ -this->x, -this->y, -this->z };
    }

    Vector3D& operator+=(const Vector3D& v) {
        this->x = this->x + v.x;
        this->y = this->y + v.y;
        this->z = this->z + v.z;
        return *this;
    }

    float Length() const {
        return std::sqrtf(std::powf(x, 2) + std::powf(y, 2) + std::powf(z, 2));
    }

    void Normalize() {
        float l = this->Length();
        this->x /= l;
        this->y /= l;
        this->z /= l;
    }
};

struct Vector4D {
    Vector4D() {}
    constexpr Vector4D(float x, float y, float z, float w = 1.0f) : x(x), y(y), z(z), w(w) {}
    constexpr Vector4D(const Vector3D vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f) {}
    float x, y, z, w;

    static constexpr Vector4D Zero() { return Vector4D(0.0f, 0.0f, 0.0f); }
};
