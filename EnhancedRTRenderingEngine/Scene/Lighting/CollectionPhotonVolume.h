#pragma once
#include "Scene/SceneObject.h"
#include "Structure/Structure.h"

class CollectionPhotonVolume : SceneObject
{
public:
    CollectionPhotonVolume(Size3Dd box) : box(box) {}
    bool Contains(Vector3D pos);

protected:
    Size3Dd box;
};

