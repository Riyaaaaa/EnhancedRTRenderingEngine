#pragma once

#include <set>

#include "Algorithms/SpaceOctree.h"

#include "Structure/Octree.h"
#include "Structure/Vector.h"
#include "Structure/Primitive.h"
#include "Structure/RaytraceStructures.h"

class RayTracer
{
public:
};

std::vector<Segment> RayTrace(SpaceOctree::OctreeFactoryBase* factory, Ray ray, unsigned int traceLevel);
std::vector<Segment> RayTraceIf(SpaceOctree::OctreeFactoryBase* factory, Ray ray, std::function<bool(const Material&)> hit_program);

std::set<uint32_t> GetColliderMortonList(SpaceOctree::OctreeFactoryBase* factory, Ray ray);
//void RayTrace(Vector3D bpos, Vector3D dir) {}

