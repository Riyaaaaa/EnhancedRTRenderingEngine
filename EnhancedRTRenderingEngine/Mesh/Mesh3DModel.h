#pragma once

#include "Resource/PMD/PMDModel.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"

class Mesh3DModel : public MeshBase<PMDVertex>
{
public:
    Mesh3DModel(const PMDModel& model);
    ~Mesh3DModel();

    std::vector<Vector3D> _speculars;
    std::vector<std::string> _materialTextures;
};

