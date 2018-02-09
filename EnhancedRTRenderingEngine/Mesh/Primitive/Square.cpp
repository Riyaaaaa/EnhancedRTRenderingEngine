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
        0, 1, 2, 1, 3, 2
    };

    _drawTargetIndexes.push_back(_indexList.size());
    _vertexCount = _indexList.size();
}

SquarePMD::SquarePMD() {
    _vertexList = std::vector<PMDVertex>{
        PMDVertex{ { -0.75f, -0.75f, +0.5f },{ 0.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
        PMDVertex{ { -0.75f, +0.75f, +0.5f },{ 0.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } },
        PMDVertex{ { +0.75f, -0.75f, +0.5f },{ 1.0f, 0.0f },{ 0.0f, 0.0f, -1.0f } },
        PMDVertex{ { +0.75f, +0.75f, +0.5f },{ 1.0f, 1.0f },{ 0.0f, 0.0f, -1.0f } }
    };

    _indexList = std::vector<uint16_t>{
        0, 1, 2, 1, 3, 2
    };

    _drawTargetIndexes.push_back(_indexList.size());
    _vertexCount = _indexList.size();
}