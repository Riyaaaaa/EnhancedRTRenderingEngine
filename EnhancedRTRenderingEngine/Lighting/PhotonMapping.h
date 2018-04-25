#pragma once

#include "Lighting/RayTracer.h"

#include "Algorithms/KDimensionalTree.h"
#include "Scene/Scene.h"
#include "Structure/Photon.h"

class PhotonMapping
{
public:
    void Compute(SpaceOctree::OctreeFactoryBase* factory, Scene* scene);

    std::vector<Line> rayPaths;
    std::vector<Photon> photons;
    KDimensionalTree<Photon> kdtree;

protected:
    void EmmitPhotons(SpaceOctree::OctreeFactoryBase* factory, Scene* scene);
    void FinalGathering(SpaceOctree::OctreeFactoryBase* factory, Scene* scene);
};

