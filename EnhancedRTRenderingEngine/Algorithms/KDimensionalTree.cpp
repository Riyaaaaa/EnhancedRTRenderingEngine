#include "stdafx.h"

#include <algorithm>

#include "KDimensionalTree.h"
#include "Utility/MathUtils.h"

template<class T>
KDNode* KDimensionalTree<T>::build(const std::vector<T>& points) {
    _points = points;
    _root = std::unique_ptr<KDNode>(_build(0, _points.size(), 0));
    return _root.get();
}

template<class T>
std::vector<std::pair<KDNode*, float>> KDimensionalTree<T>::FindNeighborNNodes(Vector3D p, int num, float limit_radius) const {
    std::vector<std::pair<KDNode*, float>> nodes;
    nodes.reserve(num);
    _FindNeighborNNodes(p, num, _root.get(), limit_radius * limit_radius, nodes);

    return nodes;
}

template<class T>
int KDimensionalTree<T>::_FindNeighborNNodes(const Vector3D& p, int num, KDNode* node, float limit_radius_sq, std::vector<std::pair<KDNode*, float>>& nodes) const {
    if (!node) {
        return 0;
    }

    int add_nums = 0;
    auto& point = _points[node->index].pos;
    float d = MathUtils::DistanceSq(p, point);
    
    if (d <= limit_radius_sq) {
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
    }
    
    float axis_d = p[static_cast<int>(node->axis)] - _points[node->index].pos[static_cast<int>(node->axis)];

    add_nums += _FindNeighborNNodes(p, num, axis_d <= 0.0f ? node->left.get() : node->right.get(), limit_radius_sq, nodes);

    if (fabs(axis_d) < limit_radius_sq) {
        add_nums += _FindNeighborNNodes(p, num, axis_d <= 0.0f ? node->right.get() : node->left.get(), limit_radius_sq, nodes);
    }

    return add_nums;
}

template<class T>
KDNode* KDimensionalTree<T>::_build(unsigned int idx, unsigned int nPoints, int depth) {
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
        return _points[lhs].pos[axis] < _points[rhs].pos[axis];
    });

    KDNode* node = new KDNode;
    node->index = index_sequence[mid];
    node->axis = static_cast<Axis>(axis);

    node->left = std::shared_ptr<KDNode>(_build(idx, mid, depth + 1));
    node->right = std::shared_ptr<KDNode>(_build(idx + mid + 1, nPoints - mid - 1, depth + 1));

    return node;
}

#include "Structure/Photon.h"

template KDimensionalTree<Photon>;
