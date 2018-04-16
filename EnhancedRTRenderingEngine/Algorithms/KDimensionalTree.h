#pragma once

#include <vector>

#include "Structure/Vector.h"
#include "Structure/KDTree.h"

class KDimensionalTree
{
public:
    //KDimensionalTree(const std::vector<Vector3D>& points) : _points(points) {}
    KDNode* build(const std::vector<Vector3D>& points);

protected:
    KDNode * _build(unsigned int idx, unsigned int nPoints, int depth);

    std::unique_ptr<KDNode> _root;
    std::vector<Vector3D> _points;
};

