#include "stdafx.h"

#include <algorithm>

#include "KDimensionalTree.h"

KDNode* KDimensionalTree::build(const std::vector<Vector3D>& points) {
    _points = points;
    _root = std::unique_ptr<KDNode>(_build(0, _points.size(), 0));
    return _root.get();
}

KDNode* KDimensionalTree::_build(unsigned int idx, unsigned int nPoints, int depth) {
    if (nPoints <= 0) {
        return nullptr;
    }

    std::vector<unsigned int> index_sequence(nPoints);
    for (int i = idx; i < nPoints + idx; i++) {
        index_sequence[i - idx] = i;
    }

    const int axis = depth % 3;
    const int mid = (nPoints - 1) / 2;

    std::nth_element(index_sequence.begin(), index_sequence.begin() + mid, index_sequence.end(), [this, axis](int lhs, int rhs) {
        return _points[lhs][axis] < _points[rhs][axis];
    });

    KDNode* node = new KDNode;
    node->index = index_sequence[mid];
    node->axis = static_cast<Axis>(axis);

    node->left = std::shared_ptr<KDNode>(_build(idx, mid, depth + 1));
    node->right = std::shared_ptr<KDNode>(_build(idx + mid + 1, nPoints - mid - 1, depth + 1));

    return node;
}
