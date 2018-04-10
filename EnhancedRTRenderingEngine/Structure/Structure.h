#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <type_traits>

#include "Vector.h"
#include "Constant/RenderConfig.h"

struct Size {
    Size(){}
    Size(float _w, float _h) : w(_w), h(_h) {}
    float w, h;
};

struct Size3D {
    float w, h, d;
    Size3D() = default;
    Size3D(float _w, float _h, float _d) : w(_w), h(_h), d(_d) {}
    Size3D operator/(float s) {
        return Size3D(w/s, h/s, d/s);
    }
};

struct Color3B {
    unsigned char r, g, b;
};

struct Color4B {
    unsigned char r, g, b, a;
};

struct BoundingBox2D {
    Vector2D pos;
    Size size;

    bool Contains(Vector2D p) const {
        return p.x >= pos.x && p.x <= pos.x + size.w &&
            p.y >= pos.y && p.y <= pos.y + size.h;
    }
};

struct BoundingBox3D {
    Vector3D pos;
    Size3D size;
};

struct AABB {
    AABB() = default;
    AABB(Vector3D _bpos, Vector3D _epos) :
    bpos(_bpos),
    epos(_epos)
    {}
    Vector3D bpos;
    Vector3D epos;
    Size3D size() const {
        auto diff = epos - bpos;
        return Size3D(diff.x, diff.y, diff.z);
    }
    Vector3D Center() const {
        Size3D halfSize = size() / 2.0f;
        return Vector3D(bpos.x + halfSize.w, bpos.y + halfSize.h, bpos.z + halfSize.d);
    }
    bool Contains(Vector3D pos) const {
        return pos.x >= bpos.x && pos.x <= epos.x &&
            pos.y >= bpos.y && pos.y <= epos.y &&
            pos.z >= bpos.z && pos.z <= epos.z ;
    }
};

struct SimpleVertex {
    Vector3D pos;    //x-y-z
    Vector4D col;    //r-g-b-a
};

struct TexVertex {
    Vector3D pos;    //x-y-z
    Vector2D tex;    //r-g-b-a
};

struct Vertex2D {
    Vector2D pos;
    Vector2D tex;
    _Vector4D<uint8_t> col;
};

struct Vertex3D {
    Vector3D pos;    //x-y-z
    Vector4D col;    //r-g-b-a
    Vector2D tex;    //x-y
};

struct LineVertex {
    Vector3D pos;    //x-y-z
    Vector4D col;    //r-g-b-a
    float thickess;
};

struct PMDVertex {
    Vector3D pos;    //x-y-z
    Vector2D tex;    //x-y
    Vector3D normal;
    Vector4D col;
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
    float Time;
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

template <class T, std::size_t Align = 16, bool = std::is_class<T>::value>
struct alignas(Align) AlignedBuffer;

template <class T, std::size_t Align>
struct alignas(Align) AlignedBuffer<T, Align, true> : public T {};

template <class T, std::size_t Align>
struct alignas(Align) AlignedBuffer<T, Align,  false> {
    T value;
};
