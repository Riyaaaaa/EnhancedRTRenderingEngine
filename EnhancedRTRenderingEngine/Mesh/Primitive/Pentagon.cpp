#include "stdafx.h"
#include "Pentagon.h"


Pentagon::Pentagon()
{
    _vertexList = std::vector<Vertex3D>{
        Vertex3D{ { +0.0f, +0.8f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f} },
        Vertex3D{ { +0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
        Vertex3D{ { -0.5f, +0.5f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
        Vertex3D{ { +0.5f, -0.5f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
        Vertex3D{ { -0.5f, -0.5f, +0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
    };

    _vertexCount = _vertexList.size();
}


Pentagon::~Pentagon()
{
}