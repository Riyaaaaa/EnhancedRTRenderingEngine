#include "stdafx.h"
#include "Square.h"

static const float      POLYGON_EDGE_LENGTH = 0.1f;
static const float      POLYGON_DEPTH = 0.5f;

Square::Square()
{
	hVectorData = std::vector<Vertex3D>{
		Vertex3D{ { -0.27f, +0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
		Vertex3D{ { +0.27f, +0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
		Vertex3D{ { -0.27f, -0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
		Vertex3D{ { +0.27f, -0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
	};

	vertexCount = hVectorData.size();
}


Square::~Square()
{
}