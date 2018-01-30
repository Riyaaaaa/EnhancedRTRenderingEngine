#include "stdafx.h"
#include "Square.h"

Square::Square()
{
	_vertexList = std::vector<Vertex3D>{
		Vertex3D{ { -0.75f, -0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
		Vertex3D{ { -0.75f, +0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
		Vertex3D{ { +0.75f, -0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
		Vertex3D{ { +0.75f, +0.75f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
	};

	_indexList = std::vector<uint16_t>{
		0, 1, 2, 3, 2, 1
	};

	_vertexCount = _indexList.size();
}


Square::~Square()
{
}
