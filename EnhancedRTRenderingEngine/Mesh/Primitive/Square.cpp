#include "stdafx.h"
#include "Square.h"

template<>
Square<Vertex3D>::Square(Size size)
{
    _vertexList = std::vector<Vertex3D>{
        Vertex3D{ { -size.w, -size.h, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
        Vertex3D{ { -size.w, -size.h, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
        Vertex3D{ { +size.w, -size.h, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
        Vertex3D{ { +size.w, +size.h, 0.0f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
    };

    _indexList = std::vector<uint16_t>{
        0, 1, 2, 1, 3, 2
    };

    _drawFacesMap.push_back(Face{ 0, _indexList.size(), 0 });
    _vertexCount = _indexList.size();
}

template<>
Square<TexVertex>::Square(Size size)
{
    _vertexList = std::vector<TexVertex>{
        TexVertex{ { -size.w, -size.h, 0.5f },{ 0.0f, 1.0f } },
        TexVertex{ { -size.w, +size.h, 0.5f },{ 0.0f, 0.0f } },
        TexVertex{ { +size.w, -size.h, 0.5f },{ 1.0f, 1.0f } },
        TexVertex{ { +size.w, +size.h, 0.5f },{ 1.0f, 0.0f } }
    };

    _indexList = std::vector<uint16_t>{
        0, 1, 2, 1, 3, 2
    };

    _drawFacesMap.push_back(Face{ 0, _indexList.size(), 0 });
    _vertexCount = _indexList.size();
}

SquarePMD::SquarePMD() {
    _vertexList = std::vector<PMDVertex>{
        PMDVertex{ { -0.75f, -0.75f, +0.5f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 0.5f, 0.5f, 1.0f } },
        PMDVertex{ { -0.75f, +0.75f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 0.5f, 0.5f, 1.0f } },
        PMDVertex{ { +0.75f, -0.75f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 0.5f, 0.5f, 1.0f } },
        PMDVertex{ { +0.75f, +0.75f, +0.5f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f },{ 0.5f, 0.5f, 0.5f, 1.0f } }
    };

    _indexList = std::vector<uint16_t>{
        0, 1, 2, 1, 3, 2
    };

    _drawFacesMap.push_back(Face{ 0,_indexList.size(), 0 });
    _vertexCount = _indexList.size();
}