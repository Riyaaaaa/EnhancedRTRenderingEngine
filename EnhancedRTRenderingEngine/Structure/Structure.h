#pragma once

#include <DirectXMath.h>
#include <cmath>

#include "Vector.h"
#include "Constant/RenderConfig.h"

struct Size {
    float w, h;
};

struct Size3D {
    float w, h, z;
};

struct Color4B {
    float r, g, b, a;
};

struct BoundingBox2D {
    Vector2D pos;
    Size size;
};

struct BoundingBox3D {
    Vector3D pos;
    Size3D size;
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