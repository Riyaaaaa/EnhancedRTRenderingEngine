#pragma once

#include <DirectXMath.h>

struct Size {
	float w, h;
};

struct Vector3D {
	float x, y, z;
};

struct Vector2D {
	float x, y;
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
	Vector3D pos;
	// X: constant attenuation factor, Y:  linear attenuation factor, Z: quadratic attenuation factor
	Vector3D attenuation;
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
	Vector3D DirectionalLight;
	PointLightParameters PointLight;
};

