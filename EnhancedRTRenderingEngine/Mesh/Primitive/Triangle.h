#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Triangle : public Mesh<Vertex3D>
{
public:
    Triangle();
};

class TrianglePMD : public Mesh<PMDVertex>
{
public:
    TrianglePMD();
};


