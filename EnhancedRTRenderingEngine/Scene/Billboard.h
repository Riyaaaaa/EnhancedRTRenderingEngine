#pragma once

#include "MeshObject.h"
#include "Structure/Structure.h"

class Billboard : public MeshObject<MainVertex>
{
public:
    const DirectX::XMMATRIX& GetMatrix() override;
	Billboard(const std::string& texture);

    void Update(Scene* scene) override;


    DirectX::XMMATRIX rot;
};
