#pragma once

#include "Lighting/RayTracer.h"

#include "Algorithms/KDimensionalTree.h"
#include "Scene/Scene.h"

class PhotonMapping
{
public:
    void Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene);

    std::vector<Line> rayPaths;
    std::vector<Vector3D> photons; //todo: change photon class
    KDimensionalTree kdtree;
};

