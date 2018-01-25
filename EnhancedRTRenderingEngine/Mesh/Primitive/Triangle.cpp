#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle()
{
	hVectorData = std::vector<Vertex3D>{
		 Vertex3D{{ +0.0f, +0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		 Vertex3D{{ +0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } },
		 Vertex3D{{ -0.5f, -0.5f, +0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f } } 
	};

	vertexCount = 3;
}


Triangle::~Triangle()
{
}
