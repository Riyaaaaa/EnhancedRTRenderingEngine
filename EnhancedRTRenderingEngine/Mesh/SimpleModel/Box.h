#pragma once
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Box : public Mesh<Vertex3D>
{
public:
    explicit Box(float size) : Box(Size3D(size, size, size))
    {
    }
    explicit Box(Size3D size);
};