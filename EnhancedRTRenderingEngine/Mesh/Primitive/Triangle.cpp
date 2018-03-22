#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle()
{
    _vertexList = std::vector<Vertex3D>{
         Vertex3D{ Vector3D{ +0.0f, +0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{0.0f, 1.0f} },
         Vertex3D{ Vector3D{ +0.5f, -0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{0.0f, 0.0f} },
         Vertex3D{ Vector3D{ -0.5f, -0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{1.0f, 0.0f} }
    };

    _vertexCount = _vertexList.size();
    _drawFacesMap.push_back(Face{ 0, _vertexList.size(), 0 });
}


Triangle::~Triangle()
{
}
