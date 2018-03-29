#pragma once

#include <set>

#include "Algorithms/SpaceOctree.h"

#include "Structure/Octree.h"
#include "Structure/Vector.h"

class RayTracer
{
public:
};

std::set<uint32_t> GetColliderMeshList(SpaceOctree::OctreeFactoryBase* factory, Vector3D bpos, Vector3D dir);
//void RayTrace(Vector3D bpos, Vector3D dir) {}

