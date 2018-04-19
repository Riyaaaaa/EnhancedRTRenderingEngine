#include "stdafx.h"

#include <algorithm>

#include "KDimensionalTree.h"

#include "Utility/MathUtils.h"

KDNode* KDimensionalTree::build(const std::vector<Vector3D>& points) {
    _points = points;
    _root = std::unique_ptr<KDNode>(_build(0, _points.size(), 0));
    return _root.get();
}

std::vector<std::pair<KDNode*, float>> KDimensionalTree::FindNeighborNNodes(Vector3D p, int num) const {
    std::vector<std::pair<KDNode*, float>> nodes;
    nodes.reserve(num);
    _FindNeighborNNodes(p, num, _root.get(), nodes);

    return nodes;
}

int KDimensionalTree::_FindNeighborNNodes(const Vector3D& p, int num, KDNode* node, std::vector<std::pair<KDNode*, float>>& nodes) const {
    if (!node) {
        return;
    }

    int add_nums = 0;
    auto& point = _points[node->index];
    float d = MathUtils::DistanceSq(p, point);

    if (num > nodes.size()) {
        auto it = std::find_if(nodes.begin(), nodes.end(), [d](std::pair<KDNode*, float> node) {
            return node.second < d;
        });
        nodes.insert(it, std::make_pair(node, d));
        add_nums = 1;
    }
    else {
        auto it = std::find_if(nodes.begin(), nodes.end(), [d](std::pair<KDNode*, float> node) {
            return node.second < d;
        });

        if (it != nodes.end()) {
            nodes.insert(it, std::make_pair(node, d));
        }

        nodes.pop_back();
    }

    float axis_d = p[static_cast<int>(node->axis)] - _points[node->index][static_cast<int>(node->axis)];

    add_nums += _FindNeighborNNodes(p, num, axis_d <= 0.0f ? node->left.get() : node->right.get(), nodes);

    if (fabs(axis_d) < nodes[0].second) {
        add_nums += _FindNeighborNNodes(p, num, axis_d <= 0.0f ? node->right.get() : node->left.get(), nodes);
    }

    return add_nums;
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
