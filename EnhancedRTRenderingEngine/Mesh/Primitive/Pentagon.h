#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

namespace PrimitiveMesh {

    class Pentagon : public Mesh<Vertex3D>
    {
    public:
        Pentagon();
        ~Pentagon();
    };

}

