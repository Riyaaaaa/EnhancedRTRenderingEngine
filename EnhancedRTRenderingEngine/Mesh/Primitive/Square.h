#pragma once

#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

template<class VertType>
class Square : public MeshBase<VertType>
{
public:
    Square(Size size);
};

class SquarePMD : public MeshBase<PMDVertex>
{
public:
    SquarePMD();
};
