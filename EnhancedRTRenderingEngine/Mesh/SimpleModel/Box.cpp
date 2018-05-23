#include "stdafx.h"
#include "Box.h"


Box::Box(float size)
{
    _vertexList = std::vector<Vertex3D>{
        { { size, size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size,-size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size, size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size, size,-size },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size, size, size },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size,-size, size },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { size,-size,-size },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size, size, size },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { size,-size,-size },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size, size,-size },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { -size, size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size,-size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { size,-size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size, size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { size,-size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size, size, size },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },

    { { -size, size,-size },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size,-size,-size },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size, size },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size, size,-size },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size, size },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size, size, size },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size, size, size },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size, size,-size },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size, size,-size },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size, size, size },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size, size,-size },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size, size, size },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size,-size,-size },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size,-size, size },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size, size },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size,-size,-size },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -size,-size, size },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size,-size,-size },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    };

    _vertexCount = static_cast<unsigned int>(_vertexList.size());
    _elements.push_back(ElementDesc{ 0, _vertexCount, 0 });
}

Box::Box(Size3D size) {
    float w = static_cast<float>(size.w);
    float h = static_cast<float>(size.h);
    float d = static_cast<float>(size.d);
    _vertexList = std::vector<Vertex3D>{
        { { w, h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w,-h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w, h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -w, h,-d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { w, h, d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w,-h, d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { w,-h,-d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w, h, d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { w,-h,-d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w, h,-d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { -w, h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -w,-h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { w,-h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -w, h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { w,-h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { w, h, d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },

    { { -w, h,-d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -w,-h,-d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h, d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -w, h,-d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h, d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -w, h, d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { w, h, d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w, h,-d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w, h,-d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { w, h, d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -w, h,-d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -w, h, d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { w,-h,-d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { w,-h, d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h, d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { w,-h,-d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -w,-h, d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -w,-h,-d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    };

    _vertexCount = static_cast<unsigned int>(_vertexList.size());
    _elements.push_back(ElementDesc{ 0, _vertexCount, 0 });
}


Box::~Box()
{
}
