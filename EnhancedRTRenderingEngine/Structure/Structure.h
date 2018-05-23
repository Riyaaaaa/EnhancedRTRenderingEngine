#pragma once

#include <DirectXMath.h>
#include <cmath>
#include <type_traits>

#include "Vector.h"
#include "Constant/RenderConfig.h"

template<class T>
struct _Size2D {
    _Size2D(){}
    _Size2D(T _w, T _h) : w(_w), h(_h) {}
    T w, h;
    _Size2D operator/(T s) {
        return _Size2D(w / s, h / s);
    }

    template<class U>
    operator _Size2D<U>() {
        return _Size2D<U>(
            static_cast<U>(w),
            static_cast<U>(h)
            );
    }
};

using Size2D = _Size2D<float>;
using Size2Dd = _Size2D<unsigned int>;

template<class T>
struct _Size3D {
    T w, h, d;
    _Size3D() = default;
    _Size3D(T _w, T _h, T _d) : w(_w), h(_h), d(_d) {}
    _Size3D operator/(T s) {
        return _Size3D(w/s, h/s, d/s);
    }

    template<class U>
    operator _Size3D<U>() {
        return _Size3D<U>(
            static_cast<U>(w),
            static_cast<U>(h),
            static_cast<U>(d)
            );
    }
};

using Size3D = _Size3D<float>;
using Size3Dd = _Size3D<unsigned int>;

struct Color3B {
    unsigned char r, g, b;
};

struct Color4B {
    unsigned char r, g, b, a;
};

struct BoundingBox2D {
    Vector2D pos;
    Size2D size;

    template<class T>
    bool Contains(_Vector2D<T> p) const {
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
    AABB operator*(float s) {
        return AABB(bpos * s, epos * s);
    }
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

struct MainVertex {
    Vector3D pos;    //x-y-z
    Vector2D tex;    //x-y
    Vector3D normal;
    Vector4D col;
    Vector2D lightUV;
};

struct Transform {
    Vector3D location;
    Vector3D rotation;
    Vector3D scale;
};

struct PointLightParameters {
    // X, Y, Z: World position, W: Inverse square attenuation radius 
    Vector4D Pos;
    // X: Intensity; Y, Z, W: Unused
    Vector4D Intensity;
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
    Vector3D baseColor;
    float metallic;
    float roughness;
    float useLightMap = -1.0f; // if negative value, don't use light map 
    float useEnviromentMap = -1.0f; // if negative value, don't use enviroment map for reflection 
};

template <class T, std::size_t Align = 16, bool = std::is_class<T>::value>
struct alignas(Align) AlignedBuffer;

template <class T, std::size_t Align>
struct alignas(Align) AlignedBuffer<T, Align, true> : public T {};

template <class T, std::size_t Align>
struct alignas(Align) AlignedBuffer<T, Align,  false> {
    T value;
};
