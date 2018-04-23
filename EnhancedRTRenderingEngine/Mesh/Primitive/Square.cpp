#include "stdafx.h"
#include "Square.h"

namespace PrimitiveMesh {

    template<>
    Square<Vertex3D>::Square(Size2D size)
    {
        float w = static_cast<float>(size.w);
        float h = static_cast<float>(size.h);
        _vertexList = std::vector<Vertex3D>{
            Vertex3D{ Vector3D{ -w, -h, 0.0f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 0.0f, 0.0f } },
            Vertex3D{ Vector3D{ -w, -h, 0.0f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 0.0f, 1.0f } },
            Vertex3D{ Vector3D{ +w, -h, 0.0f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 0.0f } },
            Vertex3D{ Vector3D{ +w, +h, 0.0f },Vector4D{ 1.0f, 1.0f, 1.0f, 1.0f },Vector2D{ 1.0f, 1.0f } }
        };

        _indexList = std::vector<uint16_t>{
            0, 1, 2, 1, 3, 2
        };

        _vertexCount = static_cast<unsigned int>(_indexList.size());
        _drawFacesMap.push_back(Face{ 0, _vertexCount, 0 });
    }

    template<>
    Square<TexVertex>::Square(Size2D size)
    {
        float w = static_cast<float>(size.w);
        float h = static_cast<float>(size.h);
        _vertexList = std::vector<TexVertex>{
            TexVertex{ Vector3D{ -w, -h, 0.5f },Vector2D{ 0.0f, 1.0f } },
            TexVertex{ Vector3D{ -w, +h, 0.5f },Vector2D{ 0.0f, 0.0f } },
            TexVertex{ Vector3D{ +w, -h, 0.5f },Vector2D{ 1.0f, 1.0f } },
            TexVertex{ Vector3D{ +w, +h, 0.5f },Vector2D{ 1.0f, 0.0f } }
        };

        _indexList = std::vector<uint16_t>{
            0, 1, 2, 1, 3, 2
        };

        _vertexCount = static_cast<unsigned int>(_indexList.size());
        _drawFacesMap.push_back(Face{ 0, _vertexCount, 0 });

    }

    SquarePMD::SquarePMD() {
        _vertexList = std::vector<MainVertex>{
            MainVertex{ Vector3D{ -0.75f, -0.75f, +0.5f },Vector2D{ 0.0f, 1.0f },Vector3D{ 0.0f, 0.0f, -1.0f },Vector4D{ 0.5f, 0.5f, 0.5f} },
            MainVertex{ Vector3D{ -0.75f, +0.75f, +0.5f },Vector2D{ 0.0f, 0.0f },Vector3D{ 0.0f, 0.0f, -1.0f },Vector4D{ 0.5f, 0.5f, 0.5f} },
            MainVertex{ Vector3D{ +0.75f, -0.75f, +0.5f },Vector2D{ 1.0f, 1.0f },Vector3D{ 0.0f, 0.0f, -1.0f },Vector4D{ 0.5f, 0.5f, 0.5f} },
            MainVertex{ Vector3D{ +0.75f, +0.75f, +0.5f },Vector2D{ 1.0f, 0.0f },Vector3D{ 0.0f, 0.0f, -1.0f },Vector4D{ 0.5f, 0.5f, 0.5f} }
        };

        _indexList = std::vector<uint16_t>{
            0, 1, 2, 1, 3, 2
        };

        _vertexCount = static_cast<unsigned int>(_indexList.size());
        _drawFacesMap.push_back(Face{ 0, _vertexCount, 0 });
    }

}