#include "stdafx.h"
#include "Triangle.h"

namespace PrimitiveMesh {

    Triangle::Triangle()
    {
        _vertexList = std::vector<Vertex3D>{
             Vertex3D{ Vector3D{ +0.0f, +0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{0.0f, 1.0f} },
             Vertex3D{ Vector3D{ +0.5f, -0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{0.0f, 0.0f} },
             Vertex3D{ Vector3D{ -0.5f, -0.5f, +0.5f }, Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f }, Vector2D{1.0f, 0.0f} }
        };

        _vertexCount = static_cast<unsigned int>(_vertexList.size());
        _elements.push_back(ElementDesc{ 0, _vertexCount, 0 });
    }

    TrianglePMD::TrianglePMD()
    {
        _vertexList = std::vector<MainVertex>{
            MainVertex{ Vector3D{ +0.0f, +0.5f, +0.5f },Vector2D{ 0.0f, 1.0f }, Vector3D{ 0.0f, 0.0f, -1.0f }, Vector4D(0.5f, 0.5f, 0.5f) },
            MainVertex{ Vector3D{ +0.5f, -0.5f, +0.5f },Vector2D{ 0.0f, 0.0f }, Vector3D{ 0.0f, 0.0f, -1.0f }, Vector4D(0.5f, 0.5f, 0.5f) },
            MainVertex{ Vector3D{ -0.5f, -0.5f, +0.5f },Vector2D{ 1.0f, 0.0f }, Vector3D{ 0.0f, 0.0f, -1.0f }, Vector4D(0.5f, 0.5f, 0.5f) }
        };

        _vertexCount = static_cast<unsigned int>(_vertexList.size());
        _elements.push_back(ElementDesc{ 0, _vertexCount, 0 });
    }

}