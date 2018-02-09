#pragma once
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Box : public MeshBase<Vertex3D>
{
public:
    Box();
    ~Box();
};