#include "stdafx.h"
#include "Pentagon.h"

namespace PrimitiveMesh {

    Pentagon::Pentagon()
    {
        _vertexList = std::vector<Vertex3D>{
            Vertex3D{ Vector3D{ +0.0f, +0.8f, +0.5f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f} },
            Vertex3D{ Vector3D{ +0.5f, +0.5f, +0.5f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f } },
            Vertex3D{ Vector3D{ -0.5f, +0.5f, +0.5f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f } },
            Vertex3D{ Vector3D{ +0.5f, -0.5f, +0.5f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f } },
            Vertex3D{ Vector3D{ -0.5f, -0.5f, +0.5f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f } }
        };

        _vertexCount = static_cast<unsigned int>(_vertexList.size());
        _elements.push_back(Face{ 0, _vertexCount, 0 });
    }


    Pentagon::~Pentagon()
    {
    }

}