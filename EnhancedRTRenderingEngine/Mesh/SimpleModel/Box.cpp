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

    _vertexCount = _vertexList.size();
    _drawFacesMap.push_back(Face{ 0, _vertexList.size(), 0 });
}

Box::Box(Size3D size) {
    _vertexList = std::vector<Vertex3D>{
        { { size.w, size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w,-size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size.w, size.h,-size.d },{ 0.0f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size.w, size.h, size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w,-size.h, size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { size.w,-size.h,-size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h, size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { size.w,-size.h,-size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h,-size.d },{ 0.5f,0.0f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { -size.w, size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size.w,-size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { size.w,-size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size.w, size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { size.w,-size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h, size.d },{ 0.0f,0.5f,0.0f,1.0f },{ 1.0f,1.0f } },

    { { -size.w, size.h,-size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size.w,-size.h,-size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h, size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size.w, size.h,-size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h, size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size.w, size.h, size.d },{ 0.5f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size.w, size.h, size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h,-size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w, size.h,-size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { size.w, size.h, size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },
    { { -size.w, size.h,-size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 0.0f,0.0f } },
    { { -size.w, size.h, size.d },{ 0.0f,0.5f,0.5f,1.0f },{ 1.0f,1.0f } },

    { { size.w,-size.h,-size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size.w,-size.h, size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h, size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { size.w,-size.h,-size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    { { -size.w,-size.h, size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 0.0f,0.0f } },
    { { -size.w,-size.h,-size.d },{ 0.0f,0.0f,0.0f,1.0f },{ 1.0f,1.0f } },
    };

    _vertexCount = _vertexList.size();
    _drawFacesMap.push_back(Face{ 0, _vertexList.size(), 0 });
}


Box::~Box()
{
}
