#pragma once

#include <vector>

#include "Structure/Vector.h"
#include "Structure/KDTree.h"

template<class T>
class KDimensionalTree
{
public:
    //KDimensionalTree(const std::vector<Vector3D>& points) : _points(points) {}
    KDNode* build(const std::vector<T>& points);
    std::vector<std::pair<KDNode*, float>> FindNeighborNNodes(Vector3D p, int num, float limit_radius) const;

protected:
    int _FindNeighborNNodes(const Vector3D& p, int num, KDNode* node, float limit_radius_sq, std::vector<std::pair<KDNode*, float>>& dist) const;

    KDNode * _build(unsigned int nPoints, int depth, std::vector<unsigned int>::iterator indices);

    std::unique_ptr<KDNode> _root;
    std::vector<T> _points;
};

