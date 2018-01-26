#pragma once

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

struct Transform {
	Vector3D location;
	Vector3D rotation;
	Vector3D scale;
};
