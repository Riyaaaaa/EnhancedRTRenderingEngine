#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

namespace PrimitiveMesh {

    template<class VertType>
    class Square : public Mesh<VertType>
    {
    public:
        Square(Size2D size);
    };

    class SquarePMD : public Mesh<MainVertex>
    {
    public:
        SquarePMD();
    };

}
