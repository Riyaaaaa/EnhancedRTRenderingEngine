#pragma once

#include <DirectXMath.h>
#include <cmath>

#include "Constant/RenderConfig.h"

struct Size {
    float w, h;
};

struct Vector3D {
    float x, y, z;

    Vector3D operator-(const Vector3D &v) const {
        return Vector3D{this->x - v.x, this->y - v.y, this->z - v.z};
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
    constexpr Vector4D(float x, float y, float z) : x(x), y(y), z(z), w(1.0f) {}
    constexpr Vector4D(const Vector3D vec3) : x(vec3.x), y(vec3.y), z(vec3.z), w(1.0f) {}
    float x, y, z, w;

    static constexpr Vector4D Zero() { return Vector4D(0.0f, 0.0f, 0.0f); }
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
    float pos[3];    //x-y-z
    float col[4];    //r-g-b-a
};

struct TexVertex {
    float pos[3];    //x-y-z
    float tex[2];    //r-g-b-a
};

struct Vertex3D {
    float pos[3];    //x-y-z
    float col[4];    //r-g-b-a
    float tex[2];    //x-y
};

struct PMDVertex {
    float pos[3];    //x-y-z
    float tex[2];    //x-y
    float normal[3];
    float col[4];
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
    DirectX::XMMATRIX DirectionalLightView[LIGHT_MAX];
    DirectX::XMMATRIX DirectionalLightProjection[LIGHT_MAX];
    DirectX::XMMATRIX PointLightView[LIGHT_MAX][6];
    DirectX::XMMATRIX PointLightProjection[LIGHT_MAX];
    Vector4D DirectionalLight[LIGHT_MAX];
    PointLightParameters PointLight[LIGHT_MAX];
    float numDirecitonalLights;
    float numPointLights;
    DirectX::XMVECTOR Eye;
};

struct ObjectBuffer
{
    DirectX::XMMATRIX World;
    DirectX::XMMATRIX NormalWorld;
};

__declspec(align(16))
struct MaterialBuffer
{
    float metallic;
    float roughness;
};

template <class T>
struct __declspec(align(16)) AlignedBuffer {
    T param;;
};