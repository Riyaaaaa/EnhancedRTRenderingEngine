#pragma once

#include "Resource/PMD/PMDModel.h"
#include "Resource/X/DXModel.h"
#include "Mesh/MeshBase.h"
#include "Structure/Structure.h"
#include "Material/Material.h"

class Mesh3DModel : public MeshBase<PMDVertex>
{
public:
    Mesh3DModel(const PMDModel& model);
	Mesh3DModel(const DXModel& model);
    ~Mesh3DModel();

    std::vector<Material> CreatePMDDefaultMaterials();

    std::vector<Vector3D> _speculars;
    std::vector<std::string> _materialTextures;
};

