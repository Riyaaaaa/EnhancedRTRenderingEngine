#pragma once
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Box : public Mesh<Vertex3D>
{
public:
    explicit Box(float size = 1.0f);
    explicit Box(Size3D size);
    ~Box();
};