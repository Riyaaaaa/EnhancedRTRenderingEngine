#pragma once

#include "Scene/MeshObject.h"

struct OctreeBox;

struct OctreeNode {
    OctreeNode(IMeshObject* _object) :
        object(_object),
        belongs(nullptr)
    {}
    IMeshObject* object;
    OctreeBox* belongs;
};

struct OctreeBox {
    std::list<std::shared_ptr<OctreeNode>> nodes;
    void Push(const std::shared_ptr<OctreeNode>& node) {
        if (node->belongs) {
            return;
        }

        nodes.push_back(node);
        node->belongs = this;
    }
};

