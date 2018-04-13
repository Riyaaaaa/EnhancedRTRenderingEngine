#pragma once

#include "RayTracer.h"

#include "Scene/Scene.h"

class PhotonMapping
{
public:
    void Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene);

    std::vector<Line> rayPaths;
};

