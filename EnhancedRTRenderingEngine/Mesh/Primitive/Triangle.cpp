#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle()
{
	hVectorData = std::array<Vertex3D, 3>{
		 Vertex3D{{ +0.0f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		 Vertex3D{{ +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		 Vertex3D{{ -0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } } };
}


Triangle::~Triangle()
{
}
