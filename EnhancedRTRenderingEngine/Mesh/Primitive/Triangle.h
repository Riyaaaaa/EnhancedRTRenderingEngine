#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

namespace PrimitiveMesh {

    class Triangle : public Mesh<Vertex3D>
    {
    public:
        Triangle();
    };

    class TrianglePMD : public Mesh<MainVertex>
    {
    public:
        TrianglePMD();
    };

}
